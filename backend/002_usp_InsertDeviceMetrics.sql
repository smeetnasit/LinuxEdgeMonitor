
CREATE OR ALTER PROCEDURE dbo.usp_InsertDeviceMetrics
    @DeviceId NVARCHAR(100),
    @TimestampUtc DATETIME2(3),
    @AgentVersion NVARCHAR(20),
    @CpuUsagePercentage DECIMAL(5,2),
    @MemoryUsagePercentage DECIMAL(5,2),
    @MemoryUsedMb BIGINT,
    @MemoryAvailableMb BIGINT,
    @DiskUsagePercentage DECIMAL(5,2),
    @DiskUsedGb DECIMAL(12,2),
    @DiskAvailableGb DECIMAL(12,2),
    @DiskReadKibPerSecond DECIMAL(12,2),
    @DiskWriteKibPerSecond DECIMAL(12,2)
AS
BEGIN
    SET NOCOUNT ON;
    SET XACT_ABORT ON;

    BEGIN TRY
        BEGIN TRANSACTION;

        IF NOT EXISTS
        (
            SELECT 1
            FROM dbo.Devices
            WHERE DeviceId = @DeviceId
        )
        BEGIN
            THROW 50002, 'Device is not registered.', 1;
        END;

        INSERT INTO dbo.DeviceMetrics
        (
            DeviceId, TimestampUtc, AgentVersion,
            CpuUsagePercentage,
            MemoryUsagePercentage, MemoryUsedMb, MemoryAvailableMb,
            DiskUsagePercentage, DiskUsedGb, DiskAvailableGb,
            DiskReadKibPerSecond, DiskWriteKibPerSecond
        )
        VALUES
        (
            @DeviceId, @TimestampUtc, @AgentVersion,
            @CpuUsagePercentage,
            @MemoryUsagePercentage, @MemoryUsedMb, @MemoryAvailableMb,
            @DiskUsagePercentage, @DiskUsedGb, @DiskAvailableGb,
            @DiskReadKibPerSecond, @DiskWriteKibPerSecond
        );

        UPDATE dbo.Devices
        SET LastSeenUtc = SYSUTCDATETIME(),
            AgentVersion = @AgentVersion
        WHERE DeviceId = @DeviceId;

        COMMIT TRANSACTION;

        SELECT SCOPE_IDENTITY() AS MetricId;
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;

        THROW;
    END CATCH;
END;
GO
