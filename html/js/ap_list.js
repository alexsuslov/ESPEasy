const renderApList = items => {
  const tbody = items.map(Tpl.Item).join('');
  return Tpl.Items(tbody);
};
// test
const aps = [['test', 10], ['test1', 50]];
// console.log(renderApList(aps));
window.onload = () => {
  console.log('page loaded!');
  const el = document.getElementById('aps');
  el.innerHTML = renderApList(aps);
};

