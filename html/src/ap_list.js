// create templates object
const Tpl = {};
// template item for access point list
const Tpl.Item = (item) => {
  return `<tr>
    <td>${item[0]}</td>
    <td>${item[1]}</td>
  </tr>`;
};
// template access point list
const Tpl.Items = (tbody) => {
  return `<table>
    <thead>
      <tr>
        <td>name</td>
        <td>rsi</td>
      </tr>
    <thead>
    <tbody>${tbody}<tbody>
  </table>`;
};

const renderApList = (items) => {
  const tbody = items
    .map(Tpl.Item)
    .join('');
  return Tpl.Items(tbody);
};
// test
const aps = [
  ['test',10],
  ['test1',50],
];
// console.log(renderApList(aps));
document.onload = () => {
  const el = document.getElementById('container');
  console.log(el);
  el.innerHTML = renderApList(aps);
}
