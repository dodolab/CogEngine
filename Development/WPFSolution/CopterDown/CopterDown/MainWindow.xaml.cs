using System.Windows;
using System.Windows.Input;
using CopterDown.Core;
using CopterDown.Core.Enums;

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
            var userAct = loop.KeysPressed;

            Act act = Act.FIRE;

            switch (e.Key)
            {
                case Key.Left:
                    act = Act.LEFT;
                    break;
                case Key.Right:
                    act = Act.RIGHT;
                    break;
                case Key.Space:
                    act = Act.FIRE;
                    break;
            }

            if(!userAct.ActionsStarted.Contains(act)) userAct.ActionsStarted.Add(act);
        }


        private void MainWindow_OnKeyUp(object sender, KeyEventArgs e)
        {
            var userAct = loop.KeysPressed;

            Act act;

            switch (e.Key)
            {
                case Key.Left:
                    act = Act.LEFT;
                    break;
                case Key.Right:
                    act = Act.RIGHT;
                    break;
                case Key.Space:
                default:
                    act = Act.FIRE;
                    break;
            }

            if (userAct.ActionsStarted.Contains(act)) userAct.ActionsStarted.Remove(act);
            if(!userAct.ActionsEnded.Contains(act)) userAct.ActionsEnded.Add(act);
        }

    }
}
