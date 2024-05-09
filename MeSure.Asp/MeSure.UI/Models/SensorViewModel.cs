namespace MeSure.UI.Models
{
    public class SensorViewModel : PadraoViewModel
    {
        public string FiwareId { get; set; }
        public string FiwareName { get; set;}
        public string SensorName { get; set;}
        public long Temperature { get; set;}
        public int Velocity { get; set;}
    }
}
