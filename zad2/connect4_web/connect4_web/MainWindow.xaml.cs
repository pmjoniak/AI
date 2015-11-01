using mshtml;
using System;
using System.Collections.Generic;
using System.IO;
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
using System.Windows.Threading;

namespace connect4_web
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		private static DispatcherTimer zombie = new DispatcherTimer();
		private static DispatcherTimer necromancer = new DispatcherTimer();

        private Board board = new Board(Board.RED);

		public MainWindow()
		{
			InitializeComponent();
			this.Loaded += MainWindow_Loaded;
			zombie.Tick += t_Tick;
			zombie.Interval = new TimeSpan(0, 0, 0, 0, 300);
			zombie.IsEnabled = false;
			necromancer.IsEnabled = false;
			necromancer.Interval = new TimeSpan(0, 0, 0, 0, 700);
			necromancer.Tick += necromancer_Tick;
		}

		void necromancer_Tick(object sender, EventArgs e)
		{
			zombie.IsEnabled = true;
			necromancer.IsEnabled = false;
		}

		void t_Tick(object sender, EventArgs e)
		{
			var document = (IHTMLDocument3)web_browser.Document;

			var last = document.getElementsByName("last_dropIt")
						.OfType<IHTMLElement>()
						.Select(element => element.getAttribute("value"))
						.FirstOrDefault();

			var value =	document.getElementsByName("texter")
						.OfType<IHTMLElement>()
						.Select(element => element.getAttribute("value"))
						.FirstOrDefault();
            int idx_tmp;
			if (last != null && last != "")
			{
				int info = int.Parse(last);
				if (info == 8)
					return;
                if (info == 7)
                    board.clear(Board.RED);
                else
                    board.move(info, out idx_tmp);
			}

			if(value == "User's turn.")
			{
                PerfectMove pm = new PerfectMove(board);
                int idx = pm.findPerfectMove(Board.RED, 5);
                board.move(idx, out idx_tmp);
				web_browser.InvokeScript("dropIt", idx);
				zombie.IsEnabled = false;
				necromancer.IsEnabled = true;
			}
		}

		void MainWindow_Loaded(object sender, RoutedEventArgs e)
		{
            var dir = Directory.GetCurrentDirectory();
			web_browser.Navigate(dir + "/../../../gra/iconnect4.html");
			web_browser.Navigated += web_browser_Navigated;
		}

		void web_browser_Navigated(object sender, NavigationEventArgs e)
		{
			zombie.IsEnabled = true;

		}
	}
}
