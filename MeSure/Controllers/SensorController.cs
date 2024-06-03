using MeSure.DAO;
using MeSure.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using System.Collections.Generic;
using System;
using System.Data.SqlTypes;
using MeSure.UI.Fiware;
using System.Threading.Tasks;
using MeSure.Fiware;
using System.Linq;

namespace MeSure.Controllers
{
    public class SensorController : PadraoController<SensorViewModel>
    {
        public SensorController()
        {
            DAO = new SensorDAO();
            GeraProximoId = true;
        }

        public override IActionResult Index()
        {
            PreencheDadosParaIndex();
            return base.Index();
        }

        protected override void ValidaDados(SensorViewModel model, string operacao)
        {
            base.ValidaDados(model, operacao);
            if (string.IsNullOrEmpty(model.NomeSensor))
                ModelState.AddModelError("NomeSensor", "Campo obrigatório.");
            if (string.IsNullOrEmpty(model.UnidadeMedida))
                ModelState.AddModelError("UnidadeMedida", "Campo obrigatório.");
            if (model.TempMax <= 0)
                ModelState.AddModelError("TempMax", "Campo obrigatório.");
            if (model.TempMin <= 0)
                ModelState.AddModelError("TempMin", "Campo obrigatório.");
            if (model.IdEstufa == 0)
                ModelState.AddModelError("IdEstufa", "Campo obrigatório.");
        }

        protected override void PreencheDadosParaView(string Operacao, SensorViewModel model)
        {
            base.PreencheDadosParaView(Operacao, model);

            EstufaDAO estufaDAO = new EstufaDAO();
            var estufas = estufaDAO.Listagem();
            List<SelectListItem> listEstufa = new List<SelectListItem>();

            listEstufa.Add(new SelectListItem("Selecione uma estufa...", "0"));
            foreach (var estufa in estufas)
            {
                SelectListItem item = new SelectListItem(estufa.Nome, estufa.Id.ToString());
                listEstufa.Add(item);
            }
            ViewBag.Estufas = listEstufa;
        }

        public void PreencheDadosParaIndex()
        {
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
        }

        public IActionResult FiltraSensor(string sedeId)
        {
            try
            {
                SensorDAO dao = new SensorDAO();

                if (sedeId == "0")
                {
                    var lista = dao.Listagem();
                    return PartialView("pvGridSensor", lista);
                }
                else
                {
                    var lista = dao.ConsultaFiltro(sedeId);
                    return PartialView("pvGridSensor", lista);
                }
            }
            catch (Exception erro)
            {
                return Json(new { erro = true, msg = erro.Message });
            }
        }

        public async Task<IActionResult> CadastraFiware(int sensorId)
        {
            FiwareIotManagerController fiwareIotManager = new FiwareIotManagerController();
            FiwareSthCometController sthCometController = new FiwareSthCometController();

            var responseIot = await fiwareIotManager.ProvisioningDevice(sensorId);
            var responseSth = await sthCometController.SubscribeTemperature(sensorId);

            if (responseIot.StatusCode == System.Net.HttpStatusCode.Conflict)
            {
                return Json(new { erro = true, msg = "Dispositivo já cadastrado no Iot" });
            }

            if (responseIot.IsSuccessStatusCode && responseSth.IsSuccessStatusCode)
            {
                return Json(new { sucesso = true, msg = "Cadastrado com sucesso" });
            }

            if (!responseIot.IsSuccessStatusCode && responseSth.IsSuccessStatusCode)
            {
                return Json(new { sucesso = true, msg = "Cadastrado parcial contate um administrador" });
            }

            return Json(new { erro = true, msg = "Algum erro aconteceu" });
        }

        public async Task ApagarFiware(int sensorId)
        {
            FiwareIotManagerController fiwareIotManagerController = new FiwareIotManagerController();

            await fiwareIotManagerController.DeletingDevice(sensorId);
        }

        public async Task<IActionResult> DevolveValores(int sensorId)
        {
            FiwareSthCometController sthCometController = new FiwareSthCometController();

            List<Object> dadosResponse = await sthCometController.RequestTemperature(sensorId);

            if (dadosResponse.Count > 1)
            {
                Double[] valores = (double[])dadosResponse.ElementAt(0);
                if (valores.Length > 0)
                {
                    double max = valores.Max();
                    double min = valores.Min();
                    double medi = valores.Sum() / 50;
                    string media = medi.ToString("F");


                    List<string> datas = (List<string>)dadosResponse.ElementAt(1);

                    return Json(new { valores = valores, maxVal = max, minVal = min, med = media, datas = datas.ToArray() });
                }
            }

            return Json(new {erro = true });
        }

        public IActionResult Dash(int sensorId)
        {
            return View(sensorId);
        }
    }
}
