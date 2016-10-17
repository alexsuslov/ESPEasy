
  const App = {
    menu: [
      { href:'.', name: 'Main' },
      { href:'config', name: 'Config' },
      { href:'hardware', name: 'Hardware' },
      { href:'devices', name: 'Devices' },
      { href:'tools', name: 'Tools' },
    ],

    renderMenuItem: (item) => {
      return `<li class="menuItem">
        <a href="${item.href}">${item.name}</a>
      </li>`;
    },

    renderMenu: () => {
      if(this.ops.UseRules)
        this.menu.push({ href:'rules', name:'Rules' });
      const elmMenu = document.getElementById('menu')
      this.menu.forEach(item => {
        elmMenu.append(this.renderMenuItem(item));
      });
    },

    renderApItem: (item) => {
      return `<tr>
        <td>${item[0]}</td>
        <td>${item[1]}</td>
      </tr`;
    },
    renderApList: () => {
      const elTable = document.getElementById('aps');

      elTable.innerHTML=`
      <table>
        <thead>
          <tr>
            <td>name</td>
            <td>rsi</td>
          </tr>
        <thead>
        <tbody>${this.ops.}<tbody>
      </table>`;
      return this;
    },

    setTitle: () => {
      document.title = this.ops.title;
      document.getElementById('title').innerHTML = 'Welcome to ESP Easy:' +
        this.ops.title;
      return this;
    },

    addCss: () => {
      const link = document.createElement( "link" );
      link.href = this.ops.css;
      link.type = "text/css";
      link.rel = "stylesheet";
      link.media = "screen,print";
      document.getElementsByTagName( "head" )[0].appendChild( link );
      return this;
    },

    init: (ops) => {
      this.ops = ops;
      if(ops.title) this.setTitle();
      if(ops.css) this.addCss();
      if(ops.menu) this.renderMenu();
      return this;
    },
  };
