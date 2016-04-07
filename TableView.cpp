#include "TableView.h"

#include <Wt/WAbstractItemModel>

// Pixels
const int TableView::kTableHeaderRowHeight = 38;
const int TableView::kTableBodyRowHeight   = 48;
const int TableView::kTableCellPadding     = 7;
const int TableView::kScrollBarWidth       = 20;
const int TableView::kDefaultDateTimeWidth = 120;

TableView::TableView(Wt::WContainerWidget* parent)
    : Wt::WTableView(parent)
{
    setHeaderHeight(kTableHeaderRowHeight);
    setRowHeight(kTableBodyRowHeight);

    setLayoutSizeAware(true);
}

auto TableView::layoutSizeChanged(int width, int height) -> void {
    // Calculate our fixed width columns
    auto nfixed = 0;
    auto nrel = 0;

    // "fixed" number of pixels
    auto fixed = 0.0;

    for (auto col : widths_) if (col.second.unit() == Wt::WLength::Percentage) nrel++;

    auto percent_sum=0.0;
    for (auto col : widths_) {
        if (col.second.unit() != Wt::WLength::Percentage)
            fixed += col.second.toPixels();
        else
            percent_sum += col.second.value();
    }

    // Check to see if the relative columns are taking up ~100% of the
    // non-fixed width.  "Roughly" because sometimes they're intentionally off
    // by a fraction just to avoid a scroll bar
    const auto epsilon = 0.5;
    if (100.0 - percent_sum > epsilon) Wt::log("debug") << "Warning: Relative column widths do not take up 100% of the available width";

    // Columns who's width wasn't explicitly set are considered "fixed", and Wt
    // will default them to 150px or something.
    nfixed = model()->columnCount() - nrel;

    // Show scroll bar?
    bool show_scroll = visible_rows_ > 0 && model()->rowCount() > visible_rows_;

    auto remainder = width - (fixed + (kTableCellPadding * model()->columnCount()) + (show_scroll ? kScrollBarWidth : 0.0));

    for (auto col : widths_) {
        if (col.second.unit() == Wt::WLength::Percentage) {
            setColumnWidth(col.first,  Wt::WLength(col.second.value()/100.0*remainder, Wt::WLength::Pixel));
        }
    }

    // Pass the call up the chain
    Wt::WTableView::layoutSizeChanged(width, height);
}

auto TableView::setColumnWidth(int column, const Wt::WLength& width) -> void {
    // Just save the data and pass the the work up
    widths_.emplace(column, width);

    Wt::WTableView::setColumnWidth(column, width);
}

/* vim: set ts=4 sw=4 sts=4 expandtab ffs=unix : */
