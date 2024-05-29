using MeSure.UI.Models;
using Microsoft.AspNetCore.Mvc;

namespace MeSure.UI.Controllers
{
    public class SensorController : PadraoController<SensorViewModel>
    {
        public override IActionResult Index()
        {
            return View();
        }
    }
}
