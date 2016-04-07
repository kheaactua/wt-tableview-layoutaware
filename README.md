This table extends [Wt](http://www.webtoolkit.eu/wt)'s [WTable](https://www.webtoolkit.eu/wt/doc/reference/html/classWt_1_1WTableView.html) view to make it more layout aware.

I found myself writing `layoutSizeChanged` on every table I was implementing because `WTableView` doesn't accept relative column widths (and I can't even think of a situation where I'd want only absolute column widths)

Looking through these forums, I found a lot of postings frustrated with the same thing, so I wrote (attached) a class that implements layout awareness. All you have to do is extend this, and then `setColumnWidth()` will work with a `Wt::WLength` of type `Wt::WLength::Percentage`

Note, often I use these tables with a `Wt::Dbo::QueryModel`, so I tend to setup the column widths in a member function other than the constructor (`::init()`).

Example
^^^^^^^

```c++
ReportTableView::ReportTableView(Wt::WContainerWidget* parent)
    : TableView(parent) { }

auto ReportTableView::init() -> void {

    // ...

    // Column widths
    setColumnWidth(kTaskTypeColumn,       Wt::WLength(40, Wt::WLength::Percentage));
    setColumnWidth(kDateCreatedColumn,    Wt::WLength(kDefaultDateTimeWidth, Wt::WLength::Pixel));
    setColumnWidth(kDateScheduledColumn,  Wt::WLength(kDefaultDateTimeWidth, Wt::WLength::Pixel));
    setColumnWidth(kDataCollectedColumn,  Wt::WLength(kDefaultDateTimeWidth, Wt::WLength::Pixel));
    setColumnWidth(kStatusColumn,         Wt::WLength(59.9, Wt::WLength::Percentage));
    setColumnWidth(kActionColumn,         Wt::WLength(91.2, Wt::WLength::Pixel));
}
```

And make sure `init()` is called after `setModel()` is called.

This will create a table widget where 4 of the columns have an absolute width, and 2 of them have a relative width. The percentages should add to ~100, and are interpreted as "percent of non-fixed with in table"

This table also sets a height based on number of rows one wants displayed, and tries to account for the vertical scroll bar - i.e. if a table without a horizontal scroll bar exceeds it's set height, it'll still try to size the columns so that when a vertical scroll bar appears, a horizontal scroll bar doesn't.

If the TableView was implemented as a straight up table tag, everything here would be unnecessary, but it isn't..

Code isn't under any license. Happy to receive feedback.  Also [posted it back to the Wt forum](http://redmine.emweb.be/boards/1/topics/11110) as the forum has helped me out a lot.
