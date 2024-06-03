using MeSure.DAO;
using MeSure.Fiware;
using MeSure.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlTypes;
using System.Linq;
using System.Threading.Tasks;

namespace MeSure.Controllers
{
    public class EstufaController : PadraoController<EstufaViewModel>
    {
        public EstufaController() 
        {
            GeraProximoId = true;
            DAO = new EstufaDAO();
        }

        protected override void PreencheDadosParaView(string Operacao, EstufaViewModel model)
        {
            base.PreencheDadosParaView(Operacao, model);

            SedeDAO sedeDAO = new SedeDAO();
            var sedes = sedeDAO.Listagem();
            List<SelectListItem> list = new List<SelectListItem>();

            list.Add(new SelectListItem("Selecione uma sede...", "0"));
            foreach (var sede in sedes)
            {
                SelectListItem item = new SelectListItem(sede.Nome, sede.Id.ToString());
                list.Add(item);
            }
            ViewBag.Sedes = list;

            ViewBag.Data = DateTime.Now;
        }

        protected override void ValidaDados(EstufaViewModel model, string operacao)
        {
            base.ValidaDados(model, operacao);
            if (string.IsNullOrEmpty(model.StatusManutencao))
                ModelState.AddModelError("StatusManutencao", "Preencha o status.");
            if (DateTime.MaxValue < model.DataManutencao && DateTime.MinValue > model.DataManutencao)
                ModelState.AddModelError("DataManutencao", "Data inválida.");
        }

        public IActionResult ObtemDadosConsultaAvancada(string nome, string sede, DateTime dtIni, DateTime dtFin)
        {
            try
            {
                EstufaDAO dao = new EstufaDAO();
                if (string.IsNullOrEmpty(nome))
                    nome = "";
                if (string.IsNullOrEmpty(sede))
                    sede = "";
                if (dtIni.Date == Convert.ToDateTime("01/01/0001"))
                    dtIni = SqlDateTime.MinValue.Value;
                if (dtFin.Date == Convert.ToDateTime("01/01/0001"))
                    dtFin = SqlDateTime.MaxValue.Value;

                var lista = dao.ConsultaFiltro(nome, sede, dtIni, dtFin);
                return PartialView("pvGridEstufa", lista);
            }
            catch (Exception erro)
            {
                return Json(new { erro = true, msg = erro.Message });
            }
        }

        public async Task<IActionResult> DevolveValoresComDatas(int estufaId, DateTime dataDe, DateTime dataAte)
        {

            SensorDAO sensorDAO = new SensorDAO();
            List<SensorViewModel> sensores = sensorDAO.ConsultaDash(estufaId);

            FiwareSthCometController sthCometController = new FiwareSthCometController();

            List<Double[]> valores = new List<Double[]>();
            List<string> datas = new List<string>();
            List<int> ids = new List<int>();

            foreach (SensorViewModel sensor in sensores)
            {
                List<Object> dadosResponse = await sthCometController.RequestTemperatureWithDates(sensor.Id, dataDe, dataAte);
                
                if (dadosResponse.Count > 1)
                {
                    Double[] temperaturas = (double[])dadosResponse.ElementAt(0);
                    if (temperaturas.Length > 0)
                    {
                        ids.Add(sensor.Id);
                        valores.Add(temperaturas);

                        datas = (List<string>)dadosResponse.ElementAt(1);
                    }
                }
            }

            if (valores.Count() >= 1)
            {
                List<Double> maximos = new List<double>();
                List<Double> minimos = new List<double>();
                double medi = 0.0;
                
                valores.ForEach(val => {
                    maximos.Add(val.Max());
                    minimos.Add(val.Min());
                    medi = val.Sum();
                });
                medi /= (valores.Count()*50);
                string media = medi.ToString("F");

                return Json(new { valores = valores, maxVal = maximos.Max(), minVal = minimos.Min(), med = media, datas = datas, listids = ids});
            }

            return Json(new { });
        }

        public async Task<IActionResult> DevolveValores(int estufaId)
        {

            SensorDAO sensorDAO = new SensorDAO();
            List<SensorViewModel> sensores = sensorDAO.ConsultaDash(estufaId);

            FiwareSthCometController sthCometController = new FiwareSthCometController();

            List<Double[]> valores = new List<Double[]>();
            List<string> datas = new List<string>();
            List<int> ids = new List<int>();

            foreach (SensorViewModel sensor in sensores)
            {
                List<Object> dadosResponse = await sthCometController.RequestTemperature(sensor.Id);

                if (dadosResponse.Count > 1)
                {
                    Double[] temperaturas = (double[])dadosResponse.ElementAt(0);
                    if (temperaturas.Length > 0)
                    {
                        ids.Add(sensor.Id);
                        valores.Add(temperaturas);

                        datas = (List<string>)dadosResponse.ElementAt(1);
                    }
                }
            }

            if (valores.Count() >= 1)
            {
                List<Double> maximos = new List<double>();
                List<Double> minimos = new List<double>();
                double medi = 0.0;

                valores.ForEach(val => {
                    maximos.Add(val.Max());
                    minimos.Add(val.Min());
                    medi = val.Sum();
                });
                medi /= (valores.Count() * 50);
                string media = medi.ToString("F");

                return Json(new { valores = valores, maxVal = maximos.Max(), minVal = minimos.Min(), med = media, datas = datas, listids = ids });
            }

            return Json(new { });
        }

        public IActionResult Dash(int estufaId)
        {
            return View(estufaId);
        }
    }
}
