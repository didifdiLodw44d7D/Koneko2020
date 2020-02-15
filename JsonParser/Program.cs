using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JsonParser
{
    class Pair
    {
        public string key { set; get; }
        public string value { set; get; }

        public Pair(string key, string value)
        {
            this.key = key;
            this.value = value;
        }
    }
    class Program
    {
        static List<Pair> pair = new List<Pair>();
        static void Main(string[] args)
        {
            string str = "{\"match request\": \"OK\", \"size\": 3, \"board\": {\"1\": {\"row\": 3, \"col\": 3}, \"2\": {\"row\": 4, \"col\": 4}, \"3\": {\"row\": 5, \"col\": 5}}}";
            string str1 = "{\"request\":100}";

            StringBuilder element = new StringBuilder(1024);

            ParseJson(str, out element);
        }

        static string ReshapeValue(StringBuilder value, string pattern)
        {
            int len = value.Length;
            int status = 0;

            StringBuilder str = new StringBuilder(1024);

            if("key" == pattern)
            {

            }
            else if("value" == pattern)
            {
                int i = 0;
                while(i<len)
                {
                    if ('\"' == value[i] && 1 == status)
                    {
                        status = 2;
                    }
                    if (1 == status)
                    {
                        str.Append(value[i]);
                    }
                    if ('\"' == value[i] && 0 == status)
                    {
                        status = 1;
                    }
                    if(' ' == value[i] && 0 == status)
                    {

                    }

                    i++;
                }
            }

            return str.ToString();
        }

        static void GetKeyValue(string str, out StringBuilder value, out StringBuilder key)
        {
            int len = str.Length;
            int i = 0;
            int status = 0;

            StringBuilder keyCopy = new StringBuilder(1024);
            StringBuilder valueCopy = new StringBuilder(1024);

            while (i < len)
            {
                if ('\"' == str[i] && 0 == status)
                {
                    int j = 0;
                    i++;

                    while (i < len)
                    {
                        if ('\"' == str[i])
                        {
                            i++;
                            break;
                        }

                        keyCopy.Append(str[i]);
                        i++;
                        j++;
                    }

                    Console.WriteLine("key = " + keyCopy);

                    status = 2;
                }
                if ('\"' == str[i] && 1 == status)
                {
                    status = 2;
                }
                if (':' == str[i] && 2 == status)
                {
                    i++;
                    status = 3;
                }
                if (3 == status)
                {
                    int j = 0;
                    while (i < len)
                    {

                        valueCopy.Append(str[i]);
                        i++;
                        j++;
                    }

                    var Copy = ReshapeValue(valueCopy, "value");

                    Console.WriteLine("value = " + Copy);
                }

                i++;
            }

            key = keyCopy;
            value = valueCopy;
        }

        static void ParseJson(string str, out StringBuilder element)
        {
            int len = str.Length;
            int status = 0;

            int i = 0;
            int k = 0;

            int depth = 0;
            int dig = 0;

            int s = 0;

            StringBuilder elementCopy = new StringBuilder(1024);



            StringBuilder key = new StringBuilder(1024);
            StringBuilder value = new StringBuilder(1024);

            while (i < len)
            {
                if ('\"' == str[i] && 0 == status)
                {
                    dig = depth;
                    status = 1;
                }
                if ('\"' == str[i] && 1 == status)
                {
                    status = 2;
                }
                if (':' == str[i] && 2 == status)
                {
                    status = 3;
                }
                if ('{' == str[i])
                {
                    depth++;
                }
                if ('}' == str[i])
                {
                    depth--;
                }
                
                if ('}' == str[i] && 0 == depth && 2 < status)
                {
                    int j = 0;
                    while (k < i)
                    {
                        elementCopy.Append(str[k]);
                        k++;
                        j++;
                    }

                    GetKeyValue(elementCopy.ToString(), out value, out key);

                    ParseJson(value.ToString(), out elementCopy);

                    pair.Add(new Pair(key.ToString(), value.ToString()));
                    
                    i++;

                    element = elementCopy;

                    element = new StringBuilder(1024);

                    break;
                }
                
                if (',' == str[i] && 3 == status && dig == depth)
                {
                    int j = 0;
                    while (k < i)
                    {
                        elementCopy.Append(str[k]);

                        k++;
                        j++;
                    }

                    GetKeyValue(elementCopy.ToString(), out value, out key);

                    ParseJson(value.ToString(), out elementCopy);

                    pair.Add(new Pair(key.ToString(), value.ToString()));

                    i++;
                    k++;
                    status = 0;

                    element = elementCopy;

                    element = new StringBuilder(1024);
                }

                i++;
            }

            element = elementCopy;
        }
    }

}

