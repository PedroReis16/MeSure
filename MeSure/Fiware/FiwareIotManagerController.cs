using MeSure.UI.Fiware.DTO;
using Newtonsoft.Json;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System;
using Microsoft.AspNetCore.Http;

namespace MeSure.UI.Fiware
{
    public class FiwareIotManagerController
    {
        public const string FIWARE_HOST_URL = "http://13.92.235.126:4041/iot";

        public async Task<SensorAttrDTO> GetDeviceAttrs()
        {
            try
            {
                Uri apiUrl = new Uri("http://13.92.235.126:1026/v2/entities/urn:ngsi-ld:Sensor:001/attrs/luminosity");

                using (HttpClient client = new HttpClient())
                {
                    client.DefaultRequestHeaders.Add("fiware-service", "smart");
                    client.DefaultRequestHeaders.Add("fiware-servicepath", "/");
                    client.DefaultRequestHeaders.Add("accept", "application/json");
                    HttpResponseMessage response = await client.GetAsync(apiUrl);

                    if (response.IsSuccessStatusCode)
                    {
                        string jsonContent = await response.Content.ReadAsStringAsync();

                        return JsonConvert.DeserializeObject<SensorAttrDTO>(jsonContent);
                    }
                    else
                    {
                        Console.WriteLine($"Erro na requisição: {response.StatusCode}");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Erro: {ex.Message}");
            }

            return null;
        }

        public async Task<bool> HealthCheckContainer()
        {
            using (HttpClient client = new HttpClient())
            {
                Uri endpoint = new Uri(FIWARE_HOST_URL + "/about");

                HttpResponseMessage response = await client.GetAsync(endpoint);

                return response.IsSuccessStatusCode;

            }
        }

        public async Task<bool> HealthCheckServices()
        {
            using (HttpClient client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add("fiware-service", "smart");
                client.DefaultRequestHeaders.Add("fiware-servicepath", "/");

                Uri endpoint = new Uri(FIWARE_HOST_URL + "/services");

                HttpResponseMessage response = await client.GetAsync(endpoint);

                return response.IsSuccessStatusCode;

            }
        }

        public async Task<HttpResponseMessage> ProvisioningDevice(int sensorId)
        {
            using (HttpClient client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add("fiware-service", "smart");
                client.DefaultRequestHeaders.Add("fiware-servicepath", "/");

                Uri endpoint = new Uri(FIWARE_HOST_URL + "/devices");

                var newSensor = new CreateSensorDTO(sensorId);

                var dtoJson = JsonConvert.SerializeObject(newSensor);

                Console.WriteLine(dtoJson);

                var payload = new StringContent(dtoJson, Encoding.UTF8, "application/json");

                HttpResponseMessage response = await client.PostAsync(endpoint, payload);

                Console.WriteLine(response.StatusCode);

                return response;
            }
        }

        public async Task<HttpResponseMessage> DeletingDevice(int sensorId)
        {
            using (HttpClient client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add("fiware-service", "smart");
                client.DefaultRequestHeaders.Add("fiware-servicepath", "/");

                Uri endpoint = new Uri(FIWARE_HOST_URL + "/devices" + "/sensor" + sensorId);

                HttpResponseMessage response = await client.DeleteAsync(endpoint);

                Console.WriteLine(response.StatusCode);

                return response;
            }
        }
    }
}
