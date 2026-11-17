using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LinuxEdgeMonitor.Application.Interfaces;
using LinuxEdgeMonitor.Domain.Entities;

namespace LinuxEdgeMonitor.Infrastructure.Repositories
{
    public class MetricsRepository : IMetricsRepository
    {
        public Task AddAsync(DeviceMetric metric, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task<IReadOnlyList<DeviceMetric>> GetHistoryByDeviceIdAsync(string deviceId, DateTime fromUtc, DateTime toUtc, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task<DeviceMetric?> GetLatestByDeviceIdAsync(string deviceId, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }
    }
}
