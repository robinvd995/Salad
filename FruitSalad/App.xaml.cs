using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace FruitSalad
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private void FloatValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex(@"^(?:\-?[0-9]*(?:\.?[0-9]*))?$");
            e.Handled = !regex.IsMatch(e.Text);
        }
    }
}
