using Microsoft.AspNetCore.Mvc;

namespace MeSure.UI.Controllers
{
    public class LoginPageController : Controller
    {
        public IActionResult Index()
        {
            return View();
        }
    }
}
