using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Util
{
    interface IFilterable<T>
    {
        IEnumerator<T> GetFilteredEnumerator(Predicate<T> predicate);
    }
}
