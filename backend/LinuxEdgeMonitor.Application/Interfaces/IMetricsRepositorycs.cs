using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LinuxEdgeMonitor.Domain.Entities;

namespace LinuxEdgeMonitor.Application.Interfaces
{
    public interface IMetricsRepository
    {
        Task AddAsync(
            DeviceMetric metric,
            CancellationToken cancellationToken = default);

        Task<DeviceMetric?> GetLatestByDeviceIdAsync(
            string deviceId,
            CancellationToken cancellationToken = default);

        Task<IReadOnlyList<DeviceMetric>> GetHistoryByDeviceIdAsync(
            string deviceId,
            DateTime fromUtc,
            DateTime toUtc,
            CancellationToken cancellationToken = default);
    }
}
