using Newtonsoft.Json;
using System.Collections.Generic;

namespace MeSure.Fiware.DTO
{
    public class MonitorTemperatureDTO
    {
        [JsonProperty("description")]
        public string Description { get; set; }

        [JsonProperty("subject")]
        public Subject Subject { get; set; }

        [JsonProperty("notification")]
        public Notification Notification { get; set; }

        public MonitorTemperatureDTO GenerateDTO(int sensorId) 
        {
            return new MonitorTemperatureDTO
            {
                Description = "Notify STH-Comet of all Motion Sensor count changes",
                Subject = new Subject
                {
                    Entities = new List<Entity>
                {
                    new Entity { Id = "urn:ngsi-ld:Sensor:"+sensorId, Type = "Sensor" }
                },
                    Condition = new Condition
                    {
                        Attributes = new List<string> { "temperature" }
                    }
                },
                Notification = new Notification
                {
                    Http = new Http
                    {
                        Url = "http://13.92.235.126:8666/notify"
                    },
                    Attributes = new List<string> { "temperature" },
                    AttributesFormat = "legacy"
                }
            };
        }
    }

    public class Subject
    {
        [JsonProperty("entities")]
        public List<Entity> Entities { get; set; }

        [JsonProperty("condition")]
        public Condition Condition { get; set; }
    }

    public class Entity
    {
        [JsonProperty("id")]
        public string Id { get; set; }

        [JsonProperty("type")]
        public string Type { get; set; }
    }

    public class Condition
    {
        [JsonProperty("attrs")]
        public List<string> Attributes { get; set; }
    }

    public class Notification
    {
        [JsonProperty("http")]
        public Http Http { get; set; }

        [JsonProperty("attrs")]
        public List<string> Attributes { get; set; }

        [JsonProperty("attrsFormat")]
        public string AttributesFormat { get; set; }
    }

    public class Http
    {
        [JsonProperty("url")]
        public string Url { get; set; }
    }

}
