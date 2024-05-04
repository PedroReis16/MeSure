using MeSure.ASP.Fiware.DTO;

namespace MeSure.ASP.Fiware
{
    public class FiwareIntegrationController
    {
        public const string FIWARE_HOST_URL = "http://13.92.235.126";

        public async Task HealthCheckIotAgent()
        {
            try
            {
                string apiUrl = FIWARE_HOST_URL + ":1026/v2/entities/urn:ngsi-ld:Sensor:001/attrs/luminosity"; // Substitua pela URL real da API

                using (HttpClient client = new HttpClient())
                {
                    HttpResponseMessage response = await client.GetAsync(apiUrl);

                    if (response.IsSuccessStatusCode)
                    {
                        string jsonContent = await response.Content.ReadAsStringAsync();

                        // Desserializa o JSON para a classe MyDto
                        SensorAttrDTO dto = JsonConvert.DeserializeObject<SensorAttrDTO>(jsonContent);

                        // Exibe os valores
                        Console.WriteLine($"Type: {dto.Type}");
                        Console.WriteLine($"Value: {dto.Value}");
                        Console.WriteLine($"TimeInstant: {dto.TimeInstant}");
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
        }
    }
}
