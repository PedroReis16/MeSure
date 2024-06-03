using MeSure.Models;
using Microsoft.AspNetCore.Diagnostics;
using NodaTime;
using System;
using System.Security.Policy;

namespace MeSure.Models
{
    public class EstufaViewModel : PadraoViewModel
    {
        public int IdSede { get; set; }
        public string Nome { get; set; }
        public string StatusManutencao { get; set; }
        public DateTime DataManutencao { get; set; }



        public string NomeSede { get; set; }
    }
}
