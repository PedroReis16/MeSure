using Newtonsoft.Json;
using System.Collections.Generic;

namespace MeSure.UI.Fiware.DTO
{
    public class CreateSensorDTO
    {
        [JsonProperty("devices")]
        public List<Device> Devices { get; set; }

        public CreateSensorDTO(int sensorId)
        {
            Devices = new List<Device> { new Device(sensorId) };
        }
    }

    public class Device
    {
        [JsonProperty("device_id")]
        public string Device_id { get; set; }
        [JsonProperty("entity_name")]
        public string Entity_name { get; set; }
        [JsonProperty("entity_type")]
        public string Entity_type { get; set; }
        [JsonProperty("protocol")]
        public string Protocol { get; set; }
        [JsonProperty("transport")]
        public string Transport { get; set; }
        [JsonProperty("commands")]
        public List<Commands> Commands { get; set; }
        [JsonProperty("attributes")]
        public List<Attributes> Attributes { get; set; }


        public Device(int sensorId)
        {
            Device_id = "sensor" + sensorId;
            Entity_name = "urn:ngsi-ld:Sensor:" + sensorId;
            Entity_type = "Sensor";
            Protocol = "PDI-IoTA-UltraLight";
            Transport = "MQTT";
            Commands = new List<Commands> { 
                                            new Commands() { Name = "on", Type = "command" }, 
                                            new Commands() { Name = "off", Type = "command" },
                                            new Commands() { Name = "unity", Type = "command" },
                                            new Commands() { Name = "maxTemp", Type = "command" },
                                            new Commands() { Name = "minTemp", Type = "command" },
                                            new Commands() { Name = "sendTimeout", Type = "command" }};
            Attributes = new List<Attributes> { 
                                                new Attributes() { Object_id="s", Name="state", Type="Text"}, 
                                                new Attributes() { Object_id="t", Name="temperature", Type="Double"},
                                                new Attributes() { Object_id="u", Name="unity", Type="Text"},
                                                new Attributes() { Object_id="maxT", Name="maximumTemp", Type="Integer"},
                                                new Attributes() { Object_id="minT", Name="minimumTemp", Type="Integer"},
                                                new Attributes() { Object_id="st", Name="sendTimeout", Type="Integer"}};
        }
    }

    public class Attributes
    {
        [JsonProperty("object_id")]
        public string Object_id { get; set; }
        [JsonProperty("name")]
        public string Name { get; set; }
        [JsonProperty("type")]
        public string Type { get; set; }
    }

    public class Commands
    {
        [JsonProperty("type")]
        public string Type { get; set; }
        [JsonProperty("name")]
        public string Name { get; set; }
    }
}
