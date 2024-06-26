﻿using MeSure.Models;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;

namespace MeSure.DAO
{
    public class SedeDAO : PadraoDAO<SedeViewModel>
    {
        protected override SqlParameter[] CriaParametros(SedeViewModel model)
        {
            object imgByte = model.ImagemEmByte;
            if (imgByte == null)
                imgByte = DBNull.Value;

            SqlParameter[] parametros =
            {
                new SqlParameter("id", model.Id),
                new SqlParameter("nome", model.Nome),
                new SqlParameter("rua", model.Rua),
                new SqlParameter("cidade", model.Cidade),
                new SqlParameter("estado", model.Estado),
                new SqlParameter("bairro", model.Bairro),
                new SqlParameter("cep", model.Cep),
                new SqlParameter("telefone", model.Telefone),
                new SqlParameter("imagem", imgByte)
            };
            return parametros;
        }

        protected override SedeViewModel MontaModel(DataRow registro)
        {
            SedeViewModel sede = new SedeViewModel();
            sede.Id = Convert.ToInt32(registro["id"]);
            sede.Nome = registro["nome"].ToString();
            sede.Rua = registro["rua"].ToString();
            sede.Cidade = registro["cidade"].ToString();
            sede.Estado = registro["estado"].ToString();
            sede.Bairro = registro["bairro"].ToString();
            sede.Cep = registro["cep"].ToString();
            sede.Telefone = registro["telefone"].ToString();

            if (registro["imagem"] != DBNull.Value)
                sede.ImagemEmByte = registro["imagem"] as byte[];

            return sede;
        }

        protected override void SetTabela()
        {
            Tabela = "sede";
        }

        public List<SedeViewModel> ConsultaFiltro(string nome, string estado)
        {
            SqlParameter[] p = {
                new SqlParameter("nome", nome),
                new SqlParameter("estado", estado),
            };

            var tabela = HelperDAO.ExecutaProcSelect("spConsultaSede", p);
            var lista = new List<SedeViewModel>();

            foreach (DataRow dr in tabela.Rows)
                lista.Add(MontaModel(dr));
            return lista;
        }
    }
}
