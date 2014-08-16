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
            this.KeyDown += MainWindow_KeyDown;
            loop = new GameLoop(Dispatcher, MyCanvas, 20, 20);
            loop.Start();
        }

        void MainWindow_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Right)
            {
                // posleme 12tku, aby si to odcyhtil ten MovementBehavior
                loop.root.SendMessage(new Message(MessageCat.MODEL,TraverseMode.NOTRAV,new float[]{20,0}){TypeId=12});
            }
            if (e.Key == Key.Left)
            {
                loop.root.SendMessage(new Message(MessageCat.MODEL, TraverseMode.NOTRAV, new float[] { -20, 00 }) { TypeId = 12 });
            }
            if (e.Key == Key.Up)
            {
                loop.root.SendMessage(new Message(MessageCat.MODEL, TraverseMode.NOTRAV, new float[] { 0, -20 }) { TypeId = 12 });
            }
            if (e.Key == Key.Down)
            {
                loop.root.SendMessage(new Message(MessageCat.MODEL, TraverseMode.NOTRAV, new float[] { 0, 20 }) { TypeId = 12 });
            }
        }
    }
}
