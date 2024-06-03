using Microsoft.AspNetCore.Mvc;

namespace MeSure.Controllers
{
    public class SobreController : Controller
    {
        public IActionResult Index()
        {
            return View();
        }
    }
}
