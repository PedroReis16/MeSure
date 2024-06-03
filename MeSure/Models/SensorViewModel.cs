using System.Runtime.InteropServices.WindowsRuntime;

namespace MeSure.Models
{
    public class SensorViewModel : PadraoViewModel
    {
        public string NomeSensor { get; set; }
        public int IdEstufa { get; set; }
        public string UnidadeMedida { get; set; }
        public decimal TempMax { get; set; }
        public decimal TempMin { get; set; }
        public string FiwareName { get; set;}
        public string NomeEstufa { get; set;}
    }
}
