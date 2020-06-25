using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Util
{
    public class Logger
    {

        private static ILogger logger = null;

        public static void SetLogger(ILogger newLogger)
        {
            logger = newLogger;
        }

        public static void LogMessage(string message)
        {
            logger.Log(message);
        }

        public static void ErrorMessage(string message)
        {
            logger.Log(message);
        }
    }

    public interface ILogger
    {
        void Log(string s);
    }
}
