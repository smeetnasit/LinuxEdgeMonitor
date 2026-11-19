
using Dapper;
using Microsoft.Data.SqlClient;
using LinuxEdgeMonitor.Application.Interfaces;
using LinuxEdgeMonitor.Domain.Entities;
using System.Data;
using LinuxEdgeMonitor.Infrastructure.Data;

namespace LinuxEdgeMonitor.Infrastructure.Repositories;

public class MetricsRepository : IMetricsRepository
{
    private readonly DBContext _dbContext;

    public MetricsRepository(DBContext dbContext)
    {
        _dbContext = dbContext;
    }

    public async Task AddAsync(
        DeviceMetric metric,
        CancellationToken cancellationToken = default)
    {
        using var connection = _dbContext.CreateConnection();

        var parameters = new
        {
            metric.DeviceId,
            TimestampUtc = metric.Timestamp,
            metric.AgentVersion,

            CpuUsagePercentage = metric.Cpu.UsagePercentage,

            MemoryUsagePercentage = metric.Memory.UsagePercentage,
            metric.Memory.UsedMb,
            metric.Memory.AvailableMb,

            DiskUsagePercentage = metric.Disk.UsagePercentage,
            metric.Disk.UsedGb,
            metric.Disk.AvailableGb,
            metric.Disk.ReadKibPerSecond,
            metric.Disk.WriteKibPerSecond
        };

        await connection.ExecuteScalarAsync<long>(
            new CommandDefinition(
                "dbo.usp_InsertDeviceMetrics",
                parameters,
                commandType: CommandType.StoredProcedure,
                cancellationToken: cancellationToken));
    }

    public async Task<DeviceMetric?> GetLatestByDeviceIdAsync(
        string deviceId,
        CancellationToken cancellationToken = default)
    {
        using var connection = _dbContext.CreateConnection();

        return await connection.QuerySingleOrDefaultAsync<DeviceMetric>(
            new CommandDefinition(
                "dbo.usp_GetLatestMetricsByDeviceId",
                new { DeviceId = deviceId },
                commandType: CommandType.StoredProcedure,
                cancellationToken: cancellationToken));
    }

    public async Task<IReadOnlyList<DeviceMetric>> GetHistoryByDeviceIdAsync(
        string deviceId,
        DateTime fromUtc,
        DateTime toUtc,
        CancellationToken cancellationToken = default)
    {
        using var connection = _dbContext.CreateConnection();

        var parameters = new
        {
            DeviceId = deviceId,
            FromUtc = fromUtc,
            ToUtc = toUtc
        };

        var results = await connection.QueryAsync<DeviceMetric>(
            new CommandDefinition(
                "dbo.usp_GetMetricsHistoryByDeviceId",
                parameters,
                commandType: CommandType.StoredProcedure,
                cancellationToken: cancellationToken));

        return results.AsList();
    }
}
