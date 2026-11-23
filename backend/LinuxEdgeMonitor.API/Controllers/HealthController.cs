
using LinuxEdgeMonitor.Infrastructure.Data;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Data.SqlClient;

namespace LinuxEdgeMonitor.API.Controllers;

[ApiController]
[Route("api/health")]
public class HealthController : ControllerBase
{
    private readonly DBContext _dbContext;

    public HealthController(DBContext dbContext)
    {
        _dbContext = dbContext;
    }

    [HttpGet("database")]
    public IActionResult CheckDatabase()
    {
        try
        {
            using var connection = _dbContext.CreateConnection();

            connection.Open();

            return Ok(new
            {
                Status = "Healthy",
                Database = connection.Database
            });
        }
        catch (SqlException)
        {
            return StatusCode(503, new
            {
                Status = "Unhealthy",
                Message = "Unable to connect to SQL Server."
            });
        }
    }
}
