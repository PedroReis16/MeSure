using System;
using static MeSure.UI.Fiware.DTO.SensorAttrDTO;

namespace MeSure.UI.Fiware.DTO
{
    public class SensorAttrDTO
    {
        public string Type { get; set; }
        public int Value { get; set; }
        public Metadata Metadata { get; set; }
    }

    public class Metadata
    {
        public TimeInstant TimeInstant { get; set; }
    }

    public class TimeInstant
    {
        public string Type { get; set; }
        public DateTime Value { get; set; }
    }
}
