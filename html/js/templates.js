// create templates object
const Tpl = {
  // template item for access point list
  Item: item => {
    return `<tr>
      <td>${ item[0] }</td>
      <td>${ item[1] }</td>
    </tr>`;
  },
  // template access point list
  Items: tbody => {
    return `<table>
      <thead>
        <tr>
          <td>name</td>
          <td>rsi</td>
        </tr>
      <thead>
      <tbody>${ tbody }<tbody>
    </table>`;
  }
};

