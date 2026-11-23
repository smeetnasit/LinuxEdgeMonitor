
using Dapper;
using LinuxEdgeMonitor.Application.Interfaces;
using LinuxEdgeMonitor.Domain.Entities;
using LinuxEdgeMonitor.Infrastructure.Data;
using System.Data;

namespace LinuxEdgeMonitor.Infrastructure.Repositories;

public class MetricsRepository : IMetricsRepository
{
    private readonly DBContext _dbContext;

    public MetricsRepository(DBContext dbContext)
    {
        _dbContext = dbContext;
    }

    // INSERT METRICS
    public async Task AddAsync(
        DeviceMetric metric,
        CancellationToken cancellationToken = default)
    {
        using var connection = _dbContext.CreateConnection();

        var parameters = new DynamicParameters();

        parameters.Add("@DeviceId", metric.DeviceId);
        parameters.Add("@TimestampUtc", metric.Timestamp);
        parameters.Add("@AgentVersion", metric.AgentVersion);

        parameters.Add("@CpuUsagePercentage",
            metric.Cpu.UsagePercentage);

        parameters.Add("@MemoryUsagePercentage",
            metric.Memory.UsagePercentage);

        parameters.Add("@MemoryUsedMb", metric.Memory.UsedMb);
        parameters.Add("@MemoryAvailableMb", metric.Memory.AvailableMb);

        parameters.Add("@DiskUsagePercentage",
            metric.Disk.UsagePercentage);

        parameters.Add("@DiskUsedGb", metric.Disk.UsedGb);
        parameters.Add("@DiskAvailableGb", metric.Disk.AvailableGb);

        parameters.Add("@DiskReadKibPerSecond",
            metric.Disk.ReadKibPerSecond);

        parameters.Add("@DiskWriteKibPerSecond",
            metric.Disk.WriteKibPerSecond);

        await connection.ExecuteScalarAsync<long>(
            new CommandDefinition(
                "dbo.usp_InsertDeviceMetrics",
                parameters,
                commandType: CommandType.StoredProcedure,
                cancellationToken: cancellationToken));
    }

    // GET LATEST METRICS
    public async Task<DeviceMetric?> GetLatestByDeviceIdAsync(
        string deviceId,
        CancellationToken cancellationToken = default)
    {
        using var connection = _dbContext.CreateConnection();

        var result = await connection.QueryFirstOrDefaultAsync<MetricResult>(
            new CommandDefinition(
                "dbo.usp_GetLatestMetricsByDeviceId",
                new { DeviceId = deviceId },
                commandType: CommandType.StoredProcedure,
                cancellationToken: cancellationToken));

        return result == null ? null : MapToDeviceMetric(result);
    }

    // GET METRICS HISTORY
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

        var results = await connection.QueryAsync<MetricResult>(
            new CommandDefinition(
                "dbo.usp_GetMetricsHistoryByDeviceId",
                parameters,
                commandType: CommandType.StoredProcedure,
                cancellationToken: cancellationToken));

        return results
            .Select(MapToDeviceMetric)
            .ToList();
    }

    // MAP SQL RESULT TO DOMAIN ENTITY
    private static DeviceMetric MapToDeviceMetric(MetricResult result)
    {
        return new DeviceMetric
        {
            Timestamp = result.TimestampUtc,
            DeviceId = result.DeviceId,
            AgentVersion = result.AgentVersion,

            Cpu = new CpuMetric
            {
                UsagePercentage = result.CpuUsagePercentage
            },

            Memory = new MemoryMetric
            {
                UsagePercentage = result.MemoryUsagePercentage,
                UsedMb = result.MemoryUsedMb,
                AvailableMb = result.MemoryAvailableMb
            },

            Disk = new DiskMetric
            {
                UsagePercentage = result.DiskUsagePercentage,
                UsedGb = result.DiskUsedGb,
                AvailableGb = result.DiskAvailableGb,
                ReadKibPerSecond = result.DiskReadKibPerSecond,
                WriteKibPerSecond = result.DiskWriteKibPerSecond
            }
        };
    }

    // FLAT DATABASE RESULT MODEL
    private sealed class MetricResult
    {
        public long MetricId { get; set; }
        public string DeviceId { get; set; } = string.Empty;
        public DateTime TimestampUtc { get; set; }
        public string AgentVersion { get; set; } = string.Empty;

        public double CpuUsagePercentage { get; set; }

        public double MemoryUsagePercentage { get; set; }
        public long MemoryUsedMb { get; set; }
        public long MemoryAvailableMb { get; set; }

        public double DiskUsagePercentage { get; set; }
        public double DiskUsedGb { get; set; }
        public double DiskAvailableGb { get; set; }
        public double DiskReadKibPerSecond { get; set; }
        public double DiskWriteKibPerSecond { get; set; }
    }
}
