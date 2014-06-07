/*
 * Year/month range selector.
 */

var months = [
    'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
    'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'
    ];

var monthSelect = function() {
    var s = select();
    for(i=0; i<12; ++i) s(option({ value: i }, months[i]));
    return s;
}

var yearSelect = function() {
    var s = select();
    for(i=2006; i<=2015; ++i) s(option({ value: i }, i));
    return s;
}

exports.template = function(callback) {
    var fromYear  = yearSelect();
    var fromMonth = monthSelect();
    var toYear    = yearSelect();
    var toMonth   = monthSelect();

    form(
        {
            onsubmit: function() {
                callback(
                    {
                        'fromYear': fromYear().value,
                        'fromMonth': fromMonth().value,
                        'toYear': toYear().value,
                        'toMonth': toMonth().value
                    }
                    );
                return false;
            }
        },
        table(
            tr(
                td('Year'),
                td(fromYear),
                td('Month'),
                td(fromMonth)
              ),
            tr(
                td('Year'),
                td(toYear),
                td('Month'),
                td(toMonth)
            ),
            tr(
                td(), td(), td(),
                td(input({ type: 'submit', value: 'Go' }))
              )
            )
        );
}

