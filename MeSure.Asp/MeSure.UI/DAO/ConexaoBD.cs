using System.Data.SqlClient;
using System;

namespace MeSure.UI.DAO
{
    public class ConexaoBD
    {
        public static SqlConnection GetConnection()
        {
            String con = "DATA SOURCE=LOCALHOST;DATABASE=AulaDB; user id=sa; password=SQL123";
            SqlConnection connection = new SqlConnection(con);
            connection.Open();
            return connection;
        }
    }
}
