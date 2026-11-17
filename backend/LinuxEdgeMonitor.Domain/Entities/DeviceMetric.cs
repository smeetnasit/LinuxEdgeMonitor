using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LinuxEdgeMonitor.Domain.Entities
{
    public class DeviceMetric
    {
        public DateTime Timestamp { get; set; }

        public string DeviceId { get; set; } = string.Empty;

        public string AgentVersion { get; set; } = string.Empty;

        public CpuMetric Cpu { get; set; } = new();

        public MemoryMetric Memory { get; set; } = new();

        public DiskMetric Disk { get; set; } = new();
    }
}
