
using LinuxEdgeMonitor.Application.Interfaces;
using LinuxEdgeMonitor.Infrastructure.Data;
using LinuxEdgeMonitor.Infrastructure.Repositories;
using System.Security.Cryptography.X509Certificates;

var builder = WebApplication.CreateBuilder(args);

// Load HTTPS certificate securely from configuration
var certificatePath = builder.Configuration["LINUXEDGEMONITOR_CERT_PATH"];
var certificatePassword = builder.Configuration["LINUXEDGEMONITOR_CERT_PASSWORD"];

if (string.IsNullOrWhiteSpace(certificatePath) ||
    string.IsNullOrWhiteSpace(certificatePassword))
{
    throw new InvalidOperationException(
        "HTTPS certificate configuration is missing.");
}

var httpsCertificate = X509CertificateLoader.LoadPkcs12FromFile(
    certificatePath,
    certificatePassword,
    X509KeyStorageFlags.UserKeySet |
    X509KeyStorageFlags.PersistKeySet);

// Configure HTTPS endpoint
builder.WebHost.ConfigureKestrel(options =>
{
    options.ListenAnyIP(7069, listenOptions =>
    {
        listenOptions.UseHttps(httpsCertificate);
    });
});

// Database connection
var connectionString = builder.Configuration
    .GetConnectionString("DefaultConnection")
    ?? throw new InvalidOperationException(
        "Database connection string 'DefaultConnection' was not found.");

builder.Services.AddSingleton(
    new DBContext(connectionString));

// Repository registration
builder.Services.AddScoped<IMetricsRepository, MetricsRepository>();

// Controllers and Swagger
builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

// Configure HTTP request pipeline
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();
app.UseAuthorization();
app.MapControllers();

app.Run();
