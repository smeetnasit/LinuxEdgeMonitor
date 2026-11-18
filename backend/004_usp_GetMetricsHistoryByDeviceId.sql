
CREATE OR ALTER PROCEDURE dbo.usp_GetMetricsHistoryByDeviceId
    @DeviceId NVARCHAR(100),
    @FromUtc DATETIME2(3),
    @ToUtc DATETIME2(3)
AS
BEGIN
    SET NOCOUNT ON;

    IF @FromUtc >= @ToUtc
    BEGIN
        THROW 50003, 'FromUtc must be earlier than ToUtc.', 1;
    END;

    SELECT
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
      AND TimestampUtc >= @FromUtc
      AND TimestampUtc < @ToUtc
    ORDER BY TimestampUtc ASC, MetricId ASC;
END;
GO
