using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Koneko2020Client
{
    public class JsonResponse
    {
        public Response response { get; set; }
    }

    public class Response
    {
        public int row { get; set; }
        public int col { get; set; }
        public string value { get; set; }
    }
}
