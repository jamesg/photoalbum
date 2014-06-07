var Application = require('./application').Application;

window.onload = function() {
    var mainControl = new Application();

    document.getElementById('template_content').appendChild(
            mainControl.element()()
            );
}

