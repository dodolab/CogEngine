using System.Windows;
using System.Windows.Input;
using CopterDown.Core;

namespace CopterDown
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private GameLoop loop;

        public MainWindow()
        {
            InitializeComponent();
            this.KeyDown += MainWindow_OnKeyDown;
            this.KeyUp += MainWindow_OnKeyUp;
            loop = new GameLoop(Dispatcher, MyCanvas, 20, 20);
            loop.Start();
        }

        private void MainWindow_OnKeyDown(object sender, KeyEventArgs e)
        {
            if (!loop.KeysPressed.Contains(e.Key)) loop.KeysPressed.Add(e.Key);
        }


        private void MainWindow_OnKeyUp(object sender, KeyEventArgs e)
        {
            if (loop.KeysPressed.Contains(e.Key)) loop.KeysPressed.Remove(e.Key);
        }

    }
}
