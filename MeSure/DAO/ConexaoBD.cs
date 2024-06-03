using System;
using System.Data.SqlClient;

namespace MeSure.DAO
{
    public class ConexaoBD
    {
        public static SqlConnection GetConnection()
        {
            String con = "DATA SOURCE=13.92.235.126;DATABASE=MeSure; user id=sa; password=SA@12345";
            SqlConnection connection = new SqlConnection(con);
            connection.Open();
            return connection;
        }
    }
}
