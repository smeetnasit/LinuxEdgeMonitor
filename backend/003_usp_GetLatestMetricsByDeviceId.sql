
CREATE OR ALTER PROCEDURE dbo.usp_GetLatestMetricsByDeviceId
    @DeviceId NVARCHAR(100)
AS
BEGIN
    SET NOCOUNT ON;

    SELECT TOP (1)
        MetricId,
        DeviceId,
        TimestampUtc,
        AgentVersion,
        CpuUsagePercentage,
        MemoryUsagePercentage,
        MemoryUsedMb,
        MemoryAvailableMb,
        DiskUsagePercentage,
        DiskUsedGb,
        DiskAvailableGb,
        DiskReadKibPerSecond,
        DiskWriteKibPerSecond,
        ReceivedAtUtc
    FROM dbo.DeviceMetrics
    WHERE DeviceId = @DeviceId
    ORDER BY TimestampUtc DESC, MetricId DESC;
END;
GO
