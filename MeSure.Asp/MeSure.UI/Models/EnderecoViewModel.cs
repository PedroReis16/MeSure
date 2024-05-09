namespace MeSure.UI.Models
{
    public class EnderecoViewModel : PadraoViewModel
    {
        public string Rua {  get; set; }
        public string Bairro { get; set; }
        public string Cidade { get; set; }
        public string Estado { get; set; }
        public string CEP { get; set; }
    }
}
