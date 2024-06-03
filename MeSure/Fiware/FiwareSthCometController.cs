using MeSure.UI.Fiware.DTO;
using Newtonsoft.Json;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System;
using MeSure.Fiware.DTO;
using Microsoft.AspNetCore.Http;
using System.Collections.Generic;
using System.Net.NetworkInformation;

namespace MeSure.Fiware
{
    public class FiwareSthCometController
    {
        public const string FIWARE_HOST_URL2 = "http://13.92.235.126:1026/v2/subscriptions";
        public const string FIWARE_HOST_URL3 = "http://13.92.235.126:8666/STH/v1/contextEntities/type/Sensor/id/urn:ngsi-ld:";

        public async Task<HttpResponseMessage> SubscribeTemperature(int sensorId)
        {
            using (HttpClient client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add("fiware-service", "smart");
                client.DefaultRequestHeaders.Add("fiware-servicepath", "/");

                Uri endpoint = new Uri(FIWARE_HOST_URL2);

                var dto = new MonitorTemperatureDTO().GenerateDTO(sensorId);

                var dtoJson = JsonConvert.SerializeObject(dto);

                Console.WriteLine("JSON Payload:");
                Console.WriteLine(dtoJson);

                var payload = new StringContent(dtoJson, Encoding.UTF8, "application/json");

                Console.WriteLine("Payload Content:");
                Console.WriteLine(await payload.ReadAsStringAsync());

                try
                {
                    HttpResponseMessage response = await client.PostAsync(endpoint, payload);
                    Console.WriteLine($"Response Status Code: {response.StatusCode}");

                    if (!response.IsSuccessStatusCode)
                    {
                        string responseContent = await response.Content.ReadAsStringAsync();
                        Console.WriteLine("Response Content:");
                        Console.WriteLine(responseContent);
                    }

                    return response;
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Exception caught:");
                    Console.WriteLine(ex.Message);
                    throw;
                }
            }
        }

        public async Task<List<Object>> RequestTemperature(int sensorId)
        {

            using (HttpClient client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add("fiware-service", "smart");
                client.DefaultRequestHeaders.Add("fiware-servicepath", "/");

                Uri endpoint = new Uri(FIWARE_HOST_URL3 + "Sensor:" + sensorId + "/attributes/temperature?lastN=50");

                HttpResponseMessage response = await client.GetAsync(endpoint);

                List<Object> dados = new List<Object>();
                List<string> datas = new List<string>();
                List<Double> valores = new List<Double>();

                if (response.IsSuccessStatusCode)
                {
                    string jsonContent = await response.Content.ReadAsStringAsync();

                    var temps = JsonConvert.DeserializeObject<RequestTemperatureDTO>(jsonContent);

                    temps.contextResponses.ForEach(cr => {
                        cr.ContextElement.Attributes.ForEach(attr =>
                        {
                            attr.Values.ForEach(value =>
                            {
                                datas.Add(value.RecvTime.ToString());
                                valores.Add(value.AttrValue);
                            });
                        });
                    });
                    dados.Add(valores.ToArray());
                    dados.Add(datas);

                    return dados;
                }

                return dados;
            }
        }
        
        public async Task<List<Object>> RequestTemperatureWithDates(int sensorId, DateTime dataDe, DateTime dataAte)
        {

            using (HttpClient client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add("fiware-service", "smart");
                client.DefaultRequestHeaders.Add("fiware-servicepath", "/");

                string formattedDe = dataDe.ToString("yyyy-MM-ddTHH:mm:ss.fff");
                string formattedAte = dataAte.ToString("yyyy-MM-ddTHH:mm:ss.fff");

                Uri endpoint = new Uri(FIWARE_HOST_URL3 + "Sensor:" + sensorId + "/attributes/temperature?lastN=50&dateFrom=" + formattedDe + "&dateTo=" + formattedAte);

                HttpResponseMessage response = await client.GetAsync(endpoint);

                List<Object> dados = new List<Object>();
                List<string> datas = new List<string>();
                List<Double> valores = new List<Double>();

                if (response.IsSuccessStatusCode)
                {
                    string jsonContent = await response.Content.ReadAsStringAsync();

                    var temps = JsonConvert.DeserializeObject<RequestTemperatureDTO>(jsonContent);

                    temps.contextResponses.ForEach(cr => {
                        cr.ContextElement.Attributes.ForEach(attr =>
                        {
                            attr.Values.ForEach(value =>
                            {
                                datas.Add(value.RecvTime.ToString());
                                valores.Add(value.AttrValue);
                            });
                        });
                    });
                    dados.Add(valores.ToArray());
                    dados.Add(datas);

                    return dados;
                }

                return dados;
            }
        }
    }
}
