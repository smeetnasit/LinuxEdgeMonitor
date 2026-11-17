using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LinuxEdgeMonitor.Domain.Entities
{
    public class DiskMetric
    {
        public double UsagePercentage { get; set; }

        public double UsedGb { get; set; }

        public double AvailableGb { get; set; }

        public double ReadKibPerSecond { get; set; }

        public double WriteKibPerSecond { get; set; }
    }
}
