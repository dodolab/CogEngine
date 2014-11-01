using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Prototype.Core;

namespace Prototype
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private GameLoop _loop;

        public MainWindow()
        {
            InitializeComponent();
            _loop = new GameLoop(Dispatcher, Canvas, 20, 20);
            _loop.Start();
        }

        private void MainWindow_OnKeyDown(object sender, KeyEventArgs e)
        {
            if(!_loop.KeysPressed.Contains(e.Key)) _loop.KeysPressed.Add(e.Key);
        }


        private void MainWindow_OnKeyUp_(object sender, KeyEventArgs e)
        {
            if (_loop.KeysPressed.Contains(e.Key)) _loop.KeysPressed.Remove(e.Key);
        }

        private void MainWindow_OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            _loop.MousePosition = Mouse.GetPosition(Canvas);
        }
    }
}
