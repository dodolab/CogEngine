using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using CopterDown.Core;
using CopterDown.Core.Entities;
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

        private List<Key> keysDown = new List<Key>();

        private void MainWindow_OnKeyDown(object sender, KeyEventArgs e)
        {
            if (keysDown.Contains(e.Key)) return;

            keysDown.Add(e.Key);
            var userAct = loop.UserActions;
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
                case Key.Tab:
                    act = Act.SWITCH;
                    break;
            }

            if(!userAct.KeyActions.Any(ac => ac.ActValue == act)) userAct.KeyActions.Add(new InputAct<Act>(act));
        }


        private void MainWindow_OnKeyUp(object sender, KeyEventArgs e)
        {
            keysDown.Remove(e.Key);

            var userAct = loop.UserActions;

            Act act;

            switch (e.Key)
            {
                case Key.Left:
                    act = Act.LEFT;
                    break;
                case Key.Right:
                    act = Act.RIGHT;
                    break;
                case Key.Tab:
                    act = Act.SWITCH;
                    break;
                case Key.Space:
                default:
                    act = Act.FIRE;
                    break;
            }

            var keyAction = userAct.KeyActions.FirstOrDefault(key => key.ActValue == act);
            if (keyAction != null) keyAction.Ended = true;
        }

    }
}
