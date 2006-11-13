using System;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace NetDraw
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		public int scalex = 600;
		public int scaley = 600;
		BreadQueue br;
		bool firstDraw = true;
		Hashtable LinkPos = new Hashtable();
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Panel panel1;
		private NetDraw.BufferPictureBox pictureBox1;
		private System.Windows.Forms.NumericUpDown zoom;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.CheckBox checkBox1;
		private System.Windows.Forms.TextBox txtFile;
		private System.Windows.Forms.Timer Redraw;
		private System.Windows.Forms.Timer Feed;
		private System.Windows.Forms.CheckBox hasHeaders;
		private System.Windows.Forms.Button cmdStart;
		private System.Windows.Forms.Button cmdStop;
		private System.Windows.Forms.Label lblLoop;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.NumericUpDown nmReload;
		private System.Windows.Forms.NumericUpDown nmEvolve;
		private System.Windows.Forms.Label label5;
		private System.ComponentModel.IContainer components;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.button1 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.Redraw = new System.Windows.Forms.Timer(this.components);
			this.Feed = new System.Windows.Forms.Timer(this.components);
			this.panel1 = new System.Windows.Forms.Panel();
			this.pictureBox1 = new NetDraw.BufferPictureBox();
			this.zoom = new System.Windows.Forms.NumericUpDown();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.checkBox1 = new System.Windows.Forms.CheckBox();
			this.txtFile = new System.Windows.Forms.TextBox();
			this.hasHeaders = new System.Windows.Forms.CheckBox();
			this.cmdStart = new System.Windows.Forms.Button();
			this.cmdStop = new System.Windows.Forms.Button();
			this.lblLoop = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.nmReload = new System.Windows.Forms.NumericUpDown();
			this.nmEvolve = new System.Windows.Forms.NumericUpDown();
			this.label5 = new System.Windows.Forms.Label();
			this.panel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.zoom)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.nmReload)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.nmEvolve)).BeginInit();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.button1.Location = new System.Drawing.Point(336, 64);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(120, 72);
			this.button1.TabIndex = 1;
			this.button1.Text = "Add";
			this.button1.Visible = false;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// button2
			// 
			this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.button2.Location = new System.Drawing.Point(336, 144);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(120, 72);
			this.button2.TabIndex = 1;
			this.button2.Text = "Remove";
			this.button2.Visible = false;
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// Redraw
			// 
			this.Redraw.Enabled = true;
			this.Redraw.Interval = 50;
			this.Redraw.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// Feed
			// 
			this.Feed.Interval = 200;
			this.Feed.Tick += new System.EventHandler(this.Feed_Tick);
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.AutoScroll = true;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.pictureBox1);
			this.panel1.Location = new System.Drawing.Point(8, 16);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(320, 344);
			this.panel1.TabIndex = 3;
			// 
			// pictureBox1
			// 
			this.pictureBox1.BackColor = System.Drawing.Color.White;
			this.pictureBox1.Location = new System.Drawing.Point(0, 0);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(323, 323);
			this.pictureBox1.TabIndex = 1;
			this.pictureBox1.PrePaint += new System.Windows.Forms.PaintEventHandler(this.pictureBox1_PrePaint);
			// 
			// zoom
			// 
			this.zoom.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.zoom.DecimalPlaces = 2;
			this.zoom.Increment = new System.Decimal(new int[] {
																   25,
																   0,
																   0,
																   131072});
			this.zoom.Location = new System.Drawing.Point(408, 240);
			this.zoom.Maximum = new System.Decimal(new int[] {
																 4,
																 0,
																 0,
																 0});
			this.zoom.Minimum = new System.Decimal(new int[] {
																 1,
																 0,
																 0,
																 65536});
			this.zoom.Name = "zoom";
			this.zoom.Size = new System.Drawing.Size(56, 20);
			this.zoom.TabIndex = 4;
			this.zoom.Value = new System.Decimal(new int[] {
															   1,
															   0,
															   0,
															   0});
			this.zoom.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.Location = new System.Drawing.Point(344, 240);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(64, 24);
			this.label1.TabIndex = 5;
			this.label1.Text = "Zoom";
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.Location = new System.Drawing.Point(344, 328);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(64, 24);
			this.label2.TabIndex = 5;
			this.label2.Text = "fps";
			// 
			// checkBox1
			// 
			this.checkBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.checkBox1.Location = new System.Drawing.Point(344, 352);
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Size = new System.Drawing.Size(104, 16);
			this.checkBox1.TabIndex = 6;
			this.checkBox1.Text = "useRect";
			this.checkBox1.Visible = false;
			this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
			// 
			// txtFile
			// 
			this.txtFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.txtFile.Location = new System.Drawing.Point(336, 16);
			this.txtFile.Name = "txtFile";
			this.txtFile.Size = new System.Drawing.Size(136, 20);
			this.txtFile.TabIndex = 7;
			this.txtFile.Text = "txtFile";
			// 
			// hasHeaders
			// 
			this.hasHeaders.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.hasHeaders.Checked = true;
			this.hasHeaders.CheckState = System.Windows.Forms.CheckState.Checked;
			this.hasHeaders.Enabled = false;
			this.hasHeaders.Location = new System.Drawing.Point(336, 40);
			this.hasHeaders.Name = "hasHeaders";
			this.hasHeaders.TabIndex = 8;
			this.hasHeaders.Text = "hasHeaders";
			// 
			// cmdStart
			// 
			this.cmdStart.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.cmdStart.Location = new System.Drawing.Point(344, 64);
			this.cmdStart.Name = "cmdStart";
			this.cmdStart.Size = new System.Drawing.Size(120, 48);
			this.cmdStart.TabIndex = 9;
			this.cmdStart.Text = "Start";
			this.cmdStart.Click += new System.EventHandler(this.cmdStart_Click);
			// 
			// cmdStop
			// 
			this.cmdStop.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.cmdStop.Location = new System.Drawing.Point(344, 128);
			this.cmdStop.Name = "cmdStop";
			this.cmdStop.Size = new System.Drawing.Size(120, 32);
			this.cmdStop.TabIndex = 10;
			this.cmdStop.Text = "Stop";
			this.cmdStop.Click += new System.EventHandler(this.cmdStop_Click);
			// 
			// lblLoop
			// 
			this.lblLoop.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.lblLoop.Location = new System.Drawing.Point(400, 224);
			this.lblLoop.Name = "lblLoop";
			this.lblLoop.Size = new System.Drawing.Size(88, 16);
			this.lblLoop.TabIndex = 11;
			this.lblLoop.Text = "0";
			// 
			// label4
			// 
			this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label4.Location = new System.Drawing.Point(344, 224);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(64, 24);
			this.label4.TabIndex = 5;
			this.label4.Text = "Loop";
			// 
			// label3
			// 
			this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label3.Location = new System.Drawing.Point(336, 272);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(104, 24);
			this.label3.TabIndex = 13;
			this.label3.Text = "Feed interval (ms)";
			// 
			// nmReload
			// 
			this.nmReload.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.nmReload.Increment = new System.Decimal(new int[] {
																	   50,
																	   0,
																	   0,
																	   0});
			this.nmReload.Location = new System.Drawing.Point(424, 272);
			this.nmReload.Maximum = new System.Decimal(new int[] {
																	 5000,
																	 0,
																	 0,
																	 0});
			this.nmReload.Minimum = new System.Decimal(new int[] {
																	 20,
																	 0,
																	 0,
																	 0});
			this.nmReload.Name = "nmReload";
			this.nmReload.Size = new System.Drawing.Size(56, 20);
			this.nmReload.TabIndex = 12;
			this.nmReload.Value = new System.Decimal(new int[] {
																   500,
																   0,
																   0,
																   0});
			this.nmReload.ValueChanged += new System.EventHandler(this.nmReload_ValueChanged);
			// 
			// nmEvolve
			// 
			this.nmEvolve.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.nmEvolve.DecimalPlaces = 2;
			this.nmEvolve.Increment = new System.Decimal(new int[] {
																	   5,
																	   0,
																	   0,
																	   131072});
			this.nmEvolve.Location = new System.Drawing.Point(416, 296);
			this.nmEvolve.Maximum = new System.Decimal(new int[] {
																	 1,
																	 0,
																	 0,
																	 0});
			this.nmEvolve.Minimum = new System.Decimal(new int[] {
																	 5,
																	 0,
																	 0,
																	 131072});
			this.nmEvolve.Name = "nmEvolve";
			this.nmEvolve.Size = new System.Drawing.Size(56, 20);
			this.nmEvolve.TabIndex = 4;
			this.nmEvolve.Value = new System.Decimal(new int[] {
																   2,
																   0,
																   0,
																   65536});
			this.nmEvolve.ValueChanged += new System.EventHandler(this.nmEvolve_ValueChanged);
			// 
			// label5
			// 
			this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label5.Location = new System.Drawing.Point(344, 296);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(64, 24);
			this.label5.TabIndex = 5;
			this.label5.Text = "Evolve speed";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(496, 373);
			this.Controls.Add(this.nmReload);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.lblLoop);
			this.Controls.Add(this.cmdStop);
			this.Controls.Add(this.cmdStart);
			this.Controls.Add(this.hasHeaders);
			this.Controls.Add(this.txtFile);
			this.Controls.Add(this.checkBox1);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.zoom);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.nmEvolve);
			this.Controls.Add(this.label5);
			this.Name = "Form1";
			this.Text = "Form1";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			this.Resize += new System.EventHandler(this.Form1_Resize);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.panel1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.zoom)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.nmReload)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.nmEvolve)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args) 
		{
			Form1 f = new Form1();
			if (args.Length > 0)
				f.txtFile.Text = args[0];
			Application.Run(f);

		}
		//ArrayList links = new ArrayList();
		Hashtable hs = new Hashtable();
		private void button1_Click(object sender, System.EventArgs e)
		{
			//CreateLink();
		}
		/*
		private void CreateLink()
		{
			Random r = new Random();
			
			Link l = null;
			bool bFound = true;
			while (bFound)
			{
				l = new Link(r.Next(br.PointList.Locations.Length-1) ,
					r.Next(br.PointList.Locations.Length-1), br);
				bFound = false;
				if (hs.Contains(l.key))
					bFound = true;
			}
			hs[l.key]=null;
			links.Add(l);
		}
*/
		private void RefreshLinks(ArrayList links)
		{
			br.Feed((Link[]) links.ToArray(typeof(Link))); 
			br.EndGeneration();
		}

		private void timer1_Tick(object sender, System.EventArgs e)
		{
			if (br != null)
			{
				br.Evolve((float)nmEvolve.Value);
				DateTime t= DateTime.Now;
				this.pictureBox1.DoDraw();
				label2.Text = (((int)(100000 / ((TimeSpan)(DateTime.Now - t)).TotalMilliseconds)) / 100F ).ToString() + " fps";;
			}
		}

		private void Feed_Tick(object sender, System.EventArgs e)
		{
			/*
			for (int n = 0; n < 200; n++)
			{
				destroyLink();
				CreateLink();
			}*/
			ArrayList links = LoadNextBlock();
			if (links != null)
				RefreshLinks(links);
		}
	
		StreamReader sr = null;
		int LinksUniquePos = -1;
		int LoopPos = -1;
		int AgentIdPos = -1;
		int YPos = -1;
		int XPos = -1;
		int headersLength = -1;
		string prevline = "";

		private void InitializeOpen()
		{
			float minX = float.MaxValue, minY = float.MaxValue, maxX = float.MinValue, maxY = float.MinValue;

			YPos = -1;
			XPos= -1;
			LinksUniquePos = -1;
			LoopPos = -1;
			AgentIdPos = -1;
			sr = new StreamReader(txtFile.Text);
			if (hasHeaders.Checked)
			{
				string header = sr.ReadLine();
				string [] headerParts = header.Split('\t');
				for (int n = 0; n < headerParts.Length;n++)
				{
					if (headerParts[n].ToLower() == "linksunique")
						LinksUniquePos = n;
					if (headerParts[n].ToLower() == "loop")
						LoopPos = n;
					if (headerParts[n].ToLower() == "x")
						XPos = n;
					if (headerParts[n].ToLower() == "y")
						YPos = n;
					if (headerParts[n].ToLower() == "agentid")
						AgentIdPos = n;
				}
				headersLength = headerParts.Length;
			}
			if (LinksUniquePos == -1)
			{
				DoStop();
				throw new Exception("Could not find the LinksUnique column at file headers");
			}
			if (LoopPos == -1)
			{
				DoStop();
				throw new Exception("Could not find the Loop column at file headers");
			}
			if (XPos == -1)
			{
				DoStop();
				throw new Exception("Could not find the X column at file headers");
			}
			if (YPos == -1)
			{
				DoStop();
				throw new Exception("Could not find the Y column at file headers");
			}
			if (AgentIdPos == -1)
			{
				DoStop();
				throw new Exception("Could not find the AgentId column at file headers");
			}
			prevline = "";
			// Lee toda una generación para ubicar los links
			br = new BreadQueue();
			br.zoom = (float) zoom.Value;
			br.useRect = checkBox1.Checked;
			br.drawHeight = pictureBox1.Height;
			br.drawWidth = pictureBox1.Width;
			PointList pl = new PointList();
			br.PointList = pl;
			// Le pone puntos
			// Trae hasta cambiar de loop
			string line = prevline;
			string [] parts;
			string links;
			int loop = -1;
			int lastLoop = -1;
			float x; float y;
			ArrayList ArrayPoints = new ArrayList();
			LinkPos = new Hashtable();
			while(true)
			{
				line = sr.ReadLine();
				parts = line.Split('\t');
				if (parts.Length < headersLength) 
					break;
				x = float.Parse(parts[XPos]);
				y = float.Parse(parts[YPos]);
				if (x< minX) minX = x;
				if (y< minY) minY = y;
				if (x> maxX) maxX = x;
				if (y> maxY) maxY = y;
				
				// Guarda
				int id = Int32.Parse(parts[AgentIdPos].Substring(2), System.Globalization.NumberStyles.AllowHexSpecifier);
				LinkPos[id] = (int)ArrayPoints.Count;
				ArrayPoints.Add(new PointF(x, y));
				// Avanza...
				loop = Int32.Parse(parts[LoopPos]);
				if (loop != lastLoop)
				{
					if (lastLoop != -1)
						break;
					lastLoop = loop;
				}
			}
			sr.Close();
			sr = new StreamReader(txtFile.Text);
			sr.ReadLine();
			lastLoop = -1;
			// Los pasa a la oficial
			pl.Locations = (PointF []) ArrayPoints.ToArray(typeof(PointF));// new Point[ArrayPoints.Count];
			/*for (int n = 0; n < ArrayPoints.Count; n++)
			{
				pl.Locations[n] = (Point) ArrayPoints[n];
			}*/
			br.minX = minX - 2;
			br.minY = minY - 2;
			br.maxX = maxX + 2;
			br.maxY = maxY + 2;
		}
		int lastLoop = -1;
			
		private ArrayList LoadNextBlock()
		{
			if (sr == null)
			{
				InitializeOpen();
			}
			// Trae hasta cambiar de loop
			string line = prevline;
			string [] parts = null;
			string links;
			int loop = -1;
			ArrayList linksArray = new ArrayList();
			while(true)
			{
				if (line.Length > 0)
				{
					parts = line.Split('\t');
					links = parts[LinksUniquePos];
					// Agrega esos links a la lista
					if (links.Length > 0 && links != "0")
					{
						string [] linksparts = links.Split(',');
						int id = Int32.Parse(parts[AgentIdPos].Substring(2), System.Globalization.NumberStyles.AllowHexSpecifier);
						for (int l = 0; l < linksparts.Length; l++)
						{
							int id2 = Int32.Parse(linksparts[l].Substring(2), System.Globalization.NumberStyles.AllowHexSpecifier);
							linksArray.Add(new Link((int) LinkPos[id], (int) LinkPos[id2], br));
						}
					}
				}
				// Lee la siguiente linea
				line = sr.ReadLine();
				if (line != null) parts = line.Split('\t');

				if (line == null || parts.Length < headersLength) 
				{
					DoStop();
					break;
				}
				loop = Int32.Parse(parts[LoopPos]);
				if (loop != lastLoop)
				{
					if (lastLoop != -1)
					{
						lastLoop = loop;
						lblLoop.Text = lastLoop.ToString();
						prevline = line;
						break;
					}
					lastLoop = loop;
				}
			}
			return linksArray;
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
			// Crea la cola
			/*
			br = new BreadQueue();
			PointList pl = new PointList();
			br.PointList = pl;
			// Le pone puntos
			int count = 10000;
			pl.Locations = new Point[count];
			Random r = new Random();
			for (int n = 0; n < count; n++)
				pl.Locations[n] = new Point(r.Next(scale), r.Next(scale));*/
			// Le pone links
/*			for (int n = 0; n < 5000; n++)
			{
				CreateLink();
			}
*/			// Listo
			scalex = panel1.ClientRectangle.Width;
			scaley = panel1.ClientRectangle.Height;

			numericUpDown1_ValueChanged(null, null);
			checkBox1_CheckedChanged(null, null);

		}

		private void pictureBox1_Click(object sender, System.EventArgs e)
		{
		
		}

		private void pictureBox1_PrePaint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
			//e.Graphics.ScaleTransform((float)zoom.Value, (float)zoom.Value);
			if (br != null)
			{
				br.Draw(e.Graphics, firstDraw);		
				firstDraw=false;
			}
			//e.Graphics.ResetTransform();
			
		}

		private void numericUpDown1_ValueChanged(object sender, System.EventArgs e)
		{
			pictureBox1.Width = (int) (scalex * zoom.Value);
			pictureBox1.Height = (int) (scaley * zoom.Value);
			if (br != null)
				br.zoom = (float) zoom.Value;
			pictureBox1.DoDraw();
		}

		private void button2_Click(object sender, System.EventArgs e)
		{
			//destroyLink();
		}
	/*	private void destroyLink()
		{
			if (links.Count == 0) return;
			Random r = new Random();
			int n = r.Next(links.Count - 1);
			hs.Remove(((Link)links[n]).key);
			links.RemoveAt(n);
			
		}*/

		private void checkBox1_CheckedChanged(object sender, System.EventArgs e)
		{
			if (br != null)
				br.useRect = checkBox1.Checked;
		}

		private void cmdStart_Click(object sender, System.EventArgs e)
		{
			firstDraw=true;
			pictureBox1.AlwaysClear = (checkBox1.Checked == false);
			pictureBox1.DoClear();
			DoStart();
		}
		private void DoPause()
		{
			cmdStart.Enabled = false;
			cmdStop.Enabled = true;
			
		}
		private void DoStop()
		{
			cmdStart.Enabled = true;
			cmdStop.Enabled = false;
			Feed.Enabled=false;
			if (sr != null)
				sr.Close();
			sr = null;
		}
		private void DoStart()
		{
			cmdStart.Enabled = false;
			Feed.Interval = (int) nmReload.Value;
			Feed.Enabled=true;
			cmdStop.Enabled = true;
		}

		private void cmdStop_Click(object sender, System.EventArgs e)
		{
			DoStop();
		}

		private void nmReload_ValueChanged(object sender, System.EventArgs e)
		{
			Feed.Interval = (int) nmReload.Value;
		}

		private void nmEvolve_ValueChanged(object sender, System.EventArgs e)
		{
		
		}

		private void Form1_Resize(object sender, System.EventArgs e)
		{
			if (br != null)
			{
				br.drawHeight = pictureBox1.Height;
				br.drawWidth = pictureBox1.Width;
			}
		}

	}
}
