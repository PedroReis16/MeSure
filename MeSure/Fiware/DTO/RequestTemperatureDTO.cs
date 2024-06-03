using Newtonsoft.Json;
using System;
using System.Collections.Generic;

namespace MeSure.Fiware.DTO
{
    public class RequestTemperatureDTO
    {
        [JsonProperty("contextResponses")]
        public List<ContextResponse> contextResponses { get; set; }
    }

    public class ContextResponse
    {
        [JsonProperty("contextElement")]
        public ContextElement ContextElement { get; set; }

        [JsonProperty("statusCode")]
        public StatusCode StatusCode { get; set; }
    }

    public class ContextElement
    {
        [JsonProperty("attributes")]
        public List<Attribute> Attributes { get; set; }

        [JsonProperty("id")]
        public string Id { get; set; }

        [JsonProperty("isPattern")]
        public bool IsPattern { get; set; }

        [JsonProperty("type")]
        public string Type { get; set; }
    }

    public class Attribute
    {
        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("values")]
        public List<Value> Values { get; set; }
    }

    public class Value
    {
        [JsonProperty("_id")]
        public string Id { get; set; }

        [JsonProperty("recvTime")]
        public DateTime RecvTime { get; set; }

        [JsonProperty("attrName")]
        public string AttrName { get; set; }

        [JsonProperty("attrType")]
        public string AttrType { get; set; }

        [JsonProperty("attrValue")]
        public double AttrValue { get; set; }
    }

    public class StatusCode
    {
        [JsonProperty("code")]
        public string Code { get; set; }

        [JsonProperty("reasonPhrase")]
        public string ReasonPhrase { get; set; }
    }



}
