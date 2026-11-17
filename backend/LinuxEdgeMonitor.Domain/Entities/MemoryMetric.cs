using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LinuxEdgeMonitor.Domain.Entities
{
    public class MemoryMetric
    {
        public double UsagePercentage { get; set; }

        public long UsedMb { get; set; }

        public long AvailableMb { get; set; }
    }
}
