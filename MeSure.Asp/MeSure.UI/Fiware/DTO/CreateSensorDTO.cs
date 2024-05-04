using Newtonsoft.Json;
using System.Collections.Generic;

namespace MeSure.UI.Fiware.DTO
{
    public class CreateSensorDTO
    {
        [JsonProperty("devices")]
        public List<Device> Devices { get; set; }

        public CreateSensorDTO()
        {
            Devices = new List<Device> { new Device() };
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


        public Device()
        {
            Device_id = "sensor003";
            Entity_name = "urn:ngsi-ld:Sensor:003";
            Entity_type = "Sensor";
            Protocol = "PDI-IoTA-UltraLight";
            Transport = "MQTT";
            Commands = new List<Commands> { new Commands() { Name = "off", Type = "command" }, new Commands() { Name = "off", Type = "command" } };
            Attributes = new List<Attributes> { new Attributes() { Object_id="s", Name="state", Type="Text"}
                                                    , new Attributes() { Object_id="l", Name="luminosity", Type="Integer"} };
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
