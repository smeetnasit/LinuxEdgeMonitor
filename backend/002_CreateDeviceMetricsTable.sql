
USE LinuxEdgeMonitorDb;
GO

CREATE TABLE dbo.DeviceMetrics
(
    MetricId BIGINT IDENTITY(1,1) NOT NULL,

    DeviceId NVARCHAR(100) NOT NULL,
    TimestampUtc DATETIME2(3) NOT NULL,
    AgentVersion NVARCHAR(20) NOT NULL,

    -- CPU metrics
    CpuUsagePercentage DECIMAL(5,2) NOT NULL,

    -- Memory metrics
    MemoryUsagePercentage DECIMAL(5,2) NOT NULL,
    MemoryUsedMb BIGINT NOT NULL,
    MemoryAvailableMb BIGINT NOT NULL,

    -- Disk metrics
    DiskUsagePercentage DECIMAL(5,2) NOT NULL,
    DiskUsedGb DECIMAL(12,2) NOT NULL,
    DiskAvailableGb DECIMAL(12,2) NOT NULL,
    DiskReadKibPerSecond DECIMAL(12,2) NOT NULL,
    DiskWriteKibPerSecond DECIMAL(12,2) NOT NULL,

    ReceivedAtUtc DATETIME2(3) NOT NULL
        CONSTRAINT DF_DeviceMetrics_ReceivedAtUtc
        DEFAULT SYSUTCDATETIME(),

    CONSTRAINT PK_DeviceMetrics
        PRIMARY KEY (MetricId)
);
GO

CREATE INDEX IX_DeviceMetrics_DeviceId_TimestampUtc
ON dbo.DeviceMetrics (DeviceId, TimestampUtc DESC);
GO
