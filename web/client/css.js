exports.css = { }

exports.css.getCssRule = function(ruleName) {
    ruleName = ruleName.toLowerCase();
    if(document.styleSheets) {
        for(var i = 0; i < document.styleSheets.length; i++) {
            var styleSheet = document.styleSheets[i];
            var ii = 0;
            var cssRule = false;
            do {
                if(styleSheet.cssRules)
                    cssRule = styleSheet.cssRules[ii];
                else
                    cssRule = styleSheet.rule[ii];
                if(cssRule)
                    if(cssRule.selectorText.toLowerCase() == ruleName)
                        return cssRule;
                ii++;
            } while(cssRule)
        }
    }
}

exports.css.deleteCssRule = function(ruleName) {
    ruleName = ruleName.toLowerCase();
    if(document.styleSheets) {
        for(var i = 0; i < document.styleSheets.length; i++) {
            var styleSheet = document.styleSheets[i];
            var ii = 0;
            var cssRule = false;
            do {
                if(styleSheet.cssRules)
                    cssRule = styleSheet.cssRules[ii];
                else
                    cssRule = styleSheet.rule[ii];
                if(cssRule) {
                    if(cssRule.selectorText.toLowerCase() == ruleName) {
                        if (styleSheet.cssRules)
                            styleSheet.deleteRule(ii);
                        else
                            styleSheet.removeRule(ii);
                    }
                }
                ii++;
            } while(cssRule)
        }
    }
}

exports.css.getOrCreateCssRule = function(ruleName) {
    var rule = exports.css.getCssRule(ruleName);
    if(rule)
        return rule;

    if (document.styleSheets[0].addRule)
        document.styleSheets[0].addRule(ruleName, null,0);
    else
        document.styleSheets[0].insertRule(ruleName+' { }', 0);

    return exports.css.getCssRule(ruleName);
}

