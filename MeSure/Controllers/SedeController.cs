using MeSure.DAO;
using MeSure.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Data.SqlTypes;
using System.IO;

namespace MeSure.Controllers
{
    public class SedeController : PadraoController<SedeViewModel>
    {
        public SedeController() 
        {
            DAO = new SedeDAO();
            GeraProximoId = true;
        }

        public byte[] ConvertImageToByte(IFormFile file)
        {
            if (file != null)
                using (var ms = new MemoryStream())
                {
                    file.CopyTo(ms);
                    return ms.ToArray();
                }
            else
                return null;
        }

        protected override void ValidaDados(SedeViewModel model, string operacao)
        {
            base.ValidaDados(model, operacao);
            if (string.IsNullOrEmpty(model.Nome))
                ModelState.AddModelError("Nome", "Preencha o nome.");
            if (string.IsNullOrEmpty(model.Estado))
                ModelState.AddModelError("Estado", "Campo Obrigatório.");
            if (string.IsNullOrEmpty(model.Cidade))
                ModelState.AddModelError("Cidade", "Campo Obrigatório.");
            if (string.IsNullOrEmpty(model.Cep))
                ModelState.AddModelError("Cep", "Campo Obrigatório.");
            if (string.IsNullOrEmpty(model.Bairro))
                ModelState.AddModelError("Bairro", "Campo Obrigatório.");
            if (string.IsNullOrEmpty(model.Telefone))
                ModelState.AddModelError("Telefone", "Campo Obrigatório.");
            if (string.IsNullOrEmpty(model.Rua))
                ModelState.AddModelError("Rua", "Campo Obrigatório.");

            //Imagem será obrigatio apenas na inclusão.
            //Na alteração iremos considerar a que já estava salva.
            if (model.Imagem == null && operacao == "I")
                ModelState.AddModelError("Imagem", "Escolha uma imagem.");
            if (model.Imagem != null && model.Imagem.Length / 1024 / 1024 >= 2)
                ModelState.AddModelError("Imagem", "Imagem limitada a 2 mb.");
            if (ModelState.IsValid)
            {
                //na alteração, se não foi informada a imagem, iremos manter a que já estava salva.
                if (operacao == "A" && model.Imagem == null)
                {
                    SedeViewModel renda = DAO.Consulta(model.Id);
                    model.ImagemEmByte = renda.ImagemEmByte;
                }
                else
                {
                    model.ImagemEmByte = ConvertImageToByte(model.Imagem);
                }
            }
        }

        public IActionResult ObtemDadosConsultaAvancada(string nome, string estado)
        {
            try
            {
                SedeDAO dao = new SedeDAO();
                if (string.IsNullOrEmpty(nome))
                    nome = "";
                if (string.IsNullOrEmpty(estado))
                    estado = "";

                var lista = dao.ConsultaFiltro(nome, estado);
                return PartialView("pvGridSede", lista);
            }
            catch (Exception erro)
            {
                return Json(new { erro = true, msg = erro.Message });
            }
        }
    }
}
