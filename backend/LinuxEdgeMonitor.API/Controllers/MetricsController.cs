
using LinuxEdgeMonitor.Application.Interfaces;
using LinuxEdgeMonitor.Domain.Entities;
using Microsoft.AspNetCore.Mvc;

namespace LinuxEdgeMonitor.API.Controllers;

[ApiController]
[Route("api/metrics")]
public class MetricsController : ControllerBase
{
    private readonly IMetricsRepository _metricsRepository;

    public MetricsController(IMetricsRepository metricsRepository)
    {
        _metricsRepository = metricsRepository;
    }

    // POST: api/metrics
    [HttpPost]
    public async Task<IActionResult> ReceiveMetrics(
        [FromBody] DeviceMetric metric,
        CancellationToken cancellationToken)
    {
        if (string.IsNullOrWhiteSpace(metric.DeviceId))
        {
            return BadRequest("DeviceId is required.");
        }

        if (string.IsNullOrWhiteSpace(metric.AgentVersion))
        {
            return BadRequest("AgentVersion is required.");
        }

        await _metricsRepository.AddAsync(metric, cancellationToken);

        return Ok(new
        {
            Message = "Metrics received successfully.",
            DeviceId = metric.DeviceId
        });
    }

    // GET: api/metrics/{deviceId}/latest
    [HttpGet("{deviceId}/latest")]
    public async Task<IActionResult> GetLatestMetrics(
        string deviceId,
        CancellationToken cancellationToken)
    {
        var metric = await _metricsRepository
            .GetLatestByDeviceIdAsync(deviceId, cancellationToken);

        if (metric == null)
        {
            return NotFound("No metrics found for this device.");
        }

        return Ok(metric);
    }

    // GET: api/metrics/{deviceId}/history
    [HttpGet("{deviceId}/history")]
    public async Task<IActionResult> GetMetricsHistory(
        string deviceId,
        [FromQuery] DateTime fromUtc,
        [FromQuery] DateTime toUtc,
        CancellationToken cancellationToken)
    {
        if (fromUtc >= toUtc)
        {
            return BadRequest("FromUtc must be earlier than ToUtc.");
        }

        var metrics = await _metricsRepository
            .GetHistoryByDeviceIdAsync(
                deviceId,
                fromUtc,
                toUtc,
                cancellationToken);

        return Ok(metrics);
    }
}
