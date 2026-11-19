
using Microsoft.Data.SqlClient;
using System.Data;

namespace LinuxEdgeMonitor.Infrastructure.Data
{
    public class DBContext
    {
        private readonly string _connectionString;

        public DBContext(string connectionString)
        {
            _connectionString = connectionString;
        }

        public IDbConnection CreateConnection()
        {
            return new SqlConnection(_connectionString);
        }
    }
}
