
CREATE OR ALTER PROCEDURE dbo.usp_RegisterDevice
    @DeviceId NVARCHAR(100),
    @Hostname NVARCHAR(255),
    @AgentVersion NVARCHAR(20)
AS
BEGIN
    SET NOCOUNT ON;

    IF EXISTS (
        SELECT 1
        FROM dbo.Devices
        WHERE DeviceId = @DeviceId
    )
    BEGIN
        THROW 50001, 'This device is already registered.', 1;
    END;

    INSERT INTO dbo.Devices
    (
        DeviceId,
        Hostname,
        AgentVersion
    )
    VALUES
    (
        @DeviceId,
        @Hostname,
        @AgentVersion
    );

    SELECT
        DeviceId,
        Hostname,
        AgentVersion,
        CreatedAtUtc,
        LastSeenUtc
    FROM dbo.Devices
    WHERE DeviceId = @DeviceId;
END;
GO
