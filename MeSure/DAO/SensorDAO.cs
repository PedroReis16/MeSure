using MeSure.Models;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;

namespace MeSure.DAO
{
    public class SensorDAO : PadraoDAO<SensorViewModel>
    {
        protected override SqlParameter[] CriaParametros(SensorViewModel model)
        {
            SqlParameter[] parametros =
            {
                new SqlParameter("id", model.Id),
                new SqlParameter("idEstufa", model.IdEstufa),
                new SqlParameter("tempMax", model.TempMax),
                new SqlParameter("tempMin", model.TempMin),
                new SqlParameter("unidadeMedida", model.UnidadeMedida),
                new SqlParameter("Nome", model.NomeSensor)
            };
            return parametros;
        }

        protected override SensorViewModel MontaModel(DataRow registro)
        {
            SensorViewModel sensor = new SensorViewModel();
            sensor.Id = Convert.ToInt32(registro["id"]);
            sensor.NomeSensor = registro["nome"].ToString();
            sensor.IdEstufa = Convert.ToInt32(registro["id_estufa"]);
            sensor.UnidadeMedida = registro["unidade_medida"].ToString();
            sensor.NomeEstufa = registro["Estufa"].ToString();
            sensor.TempMax = Convert.ToDecimal(registro["Temp_max"]);
            sensor.TempMin = Convert.ToDecimal(registro["Temp_min"]);

            return sensor;
        }

        protected override void SetTabela()
        {
            Tabela = "sensor";
            NomeSpListagem = "spListagem_Sensor";
            NomeSpConsulta = "spConsulta_Sensor";
        }

        public List<SensorViewModel> ConsultaFiltro(string sedeId)
        {
            SqlParameter[] p = {
                new SqlParameter("idSede", sedeId)
            };

            var tabela = HelperDAO.ExecutaProcSelect("spFiltraSensor", p);
            var lista = new List<SensorViewModel>();

            foreach (DataRow dr in tabela.Rows)
                lista.Add(MontaModel(dr));
            return lista;
        }

        public List<SensorViewModel> ConsultaDash(int estufaId)
        {
            SqlParameter[] p = {
                new SqlParameter("idEstufa", estufaId)
            };

            var tabela = HelperDAO.ExecutaProcSelect("spFiltraSensorEstufa", p);
            var lista = new List<SensorViewModel>();

            foreach (DataRow dr in tabela.Rows)
                lista.Add(MontaModel(dr));
            return lista;
        }
    }
}
