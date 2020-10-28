using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TrueStepTerminal
{
    public class Parameters
    {
        public class ParameterItem
        {
            public string paramName;

            public double paramValue;
            public string Name 
            { 
                get 
                { 
                    return paramName; 
                } 
                set 
                { 
                    paramName = value; 
                } 
            }
            public string Value 
            { 
                get 
                { 
                    return paramValue.ToString(); 
                } 
                set 
                {
                    double result;
                    if (double.TryParse(value.ToString(), out result))
                        paramValue = result; 
                } 
            }

            public ParameterItem()
            {

            }

            public ParameterItem(string name, double value)
            {
                paramName = name;
                paramValue = value;
            }

            public ParameterItem(string name, string value)
            {
                paramName = name;
                paramValue = double.Parse(value);
            }
        }

        public BindingList<ParameterItem> Items = new BindingList<ParameterItem>();

       
    }
}
