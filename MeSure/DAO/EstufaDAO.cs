using MeSure.Models;
using MeSure.DAO;
using System;
using System.Data;
using System.Data.SqlClient;
using NodaTime;
using System.Collections.Generic;

namespace MeSure.DAO
{
    public class EstufaDAO : PadraoDAO<EstufaViewModel>
    {
        protected override SqlParameter[] CriaParametros(EstufaViewModel model)
        {
            SqlParameter[] parametros =
            {
                new SqlParameter("id", model.Id),
                new SqlParameter("idSede", model.IdSede),
                new SqlParameter("nome", model.Nome),
                new SqlParameter("status", model.StatusManutencao),
                new SqlParameter("DataManutencao", model.DataManutencao)
            };
            return parametros;
        }

        protected override EstufaViewModel MontaModel(DataRow registro)
        {
            EstufaViewModel estufa = new EstufaViewModel();
            estufa.Id = Convert.ToInt32(registro["id"]);
            estufa.Nome = registro["nome"].ToString();
            estufa.IdSede = Convert.ToInt32(registro["id_sede"]);
            estufa.StatusManutencao = registro["Status_Manutencao"].ToString();
            estufa.DataManutencao = Convert.ToDateTime(registro["Data_Manutencao"]);
            estufa.NomeSede = registro["sede"].ToString();

            return estufa;
        }

        protected override void SetTabela()
        {
            Tabela = "estufa";
            NomeSpListagem = "spListagem_Estufa";
            NomeSpConsulta = "spConsulta_Estufa";
        }

        public List<EstufaViewModel> Listagem(int idSede)
        {
            var p = new SqlParameter[]
            {
                new SqlParameter("idSede", idSede),
            };
            var tabela = HelperDAO.ExecutaProcSelect("spListagem_Estufa", p);
            List<EstufaViewModel> lista = new List<EstufaViewModel>();
            foreach (DataRow registro in tabela.Rows)
                lista.Add(MontaModel(registro));
            return lista;
        }

        public List<EstufaViewModel> ConsultaFiltro(string nome, string sede, DateTime dtIni, DateTime dtFin)
        {
            SqlParameter[] p = {
                new SqlParameter("nome", nome),
                new SqlParameter("sede", sede),
                new SqlParameter("dataInicial", dtIni),
                new SqlParameter("dataFinal", dtFin),
            };

            var tabela = HelperDAO.ExecutaProcSelect("spFiltroEstufa", p);
            var lista = new List<EstufaViewModel>();

            foreach (DataRow dr in tabela.Rows)
                lista.Add(MontaModel(dr));
            return lista;
        }
    }
}
