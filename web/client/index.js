var Application = require('./application').Application;

window.onload = function() {
    if(!window.localStorage) {
        window.localStorage = {
            getItem: function(key) {
                return window.localStorage[key];
            },
            setItem: function(key, value) {
                window.localStorage[key] = value;
            }
        };
    }

    var mainControl = new Application();

    document.getElementById('template_content').appendChild(
            mainControl.element()()
            );
}

