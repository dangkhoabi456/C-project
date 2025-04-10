#include <gtk/gtk.h>

// Khai báo trước hàm xử lý sự kiện
static void onClicked(GtkWidget *widget, gpointer data);
static void onNhapBienSoXe(GtkWidget *widget, gpointer data);

// Hàm callback được gọi khi ứng dụng khởi động
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *label;
    GtkWidget *containerBox;

    // Tạo cửa sổ chính
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Quản lý bãi giữ xe");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    // Tạo hộp chứa theo chiều dọc
    containerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // ==== TẠO VÀ THÊM 3 NÚT GÓC TRÊN PHẢI ====
    GtkWidget *top_right_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    GtkWidget *btn1 = gtk_button_new_with_label("Thêm");
    GtkWidget *btn2 = gtk_button_new_with_label("Xóa");
    GtkWidget *btn3 = gtk_button_new_with_label("Thanh toán");

    g_signal_connect(btn1, "clicked", G_CALLBACK(onNhapBienSoXe), window);
    g_signal_connect(btn2, "clicked", G_CALLBACK(onClicked), NULL);
    g_signal_connect(btn3, "clicked", G_CALLBACK(onClicked), NULL);

    gtk_box_pack_start(GTK_BOX(top_right_box), btn1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(top_right_box), btn2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(top_right_box), btn3, FALSE, FALSE, 0);

    gtk_widget_set_halign(top_right_box, GTK_ALIGN_END);
    gtk_box_pack_start(GTK_BOX(containerBox), top_right_box, FALSE, FALSE, 5);

    // ==== LABEL ====
    label = gtk_label_new("Chào mừng tới hệ thống quản lý bãi giữ xe");
    gtk_box_pack_start(GTK_BOX(containerBox), label, FALSE, FALSE, 5);
   
       // ==== TẠO NOTEBOOK (CÁC TAB) ====
    GtkWidget *notebook = gtk_notebook_new();

    // === TAB 1: Trang chủ ===
    GtkWidget *tab_label1 = gtk_label_new("Trang chủ");
    GtkWidget *tab_content1 = gtk_label_new("Chào mừng đến với hệ thống quản lý bãi giữ xe");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_content1, tab_label1);

    // === TAB 2: Thống kê ===
    GtkWidget *tab_label2 = gtk_label_new("Thống kê");
    GtkWidget *tab_content2 = gtk_label_new("Dữ liệu thống kê xe vào/ra, doanh thu, v.v...");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_content2, tab_label2);

    // === TAB 3: Bãi xe ===
GtkWidget *tab_label3 = gtk_label_new("Bãi xe");

// Tạo notebook con để chứa các tầng
GtkWidget *nested_notebook = gtk_notebook_new();

// === Tầng 1 ===
GtkWidget *tab_tang1 = gtk_label_new("Tầng 1");

// Model và TreeView cho tầng 1
GtkListStore *store_tang1 = gtk_list_store_new(1, G_TYPE_STRING);
GtkTreeIter iter1;
gtk_list_store_append(store_tang1, &iter1);
gtk_list_store_set(store_tang1, &iter1, 0, "59A1-123.45", -1);

GtkWidget *treeview_tang1 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store_tang1));
GtkCellRenderer *renderer1 = gtk_cell_renderer_text_new();
GtkTreeViewColumn *column1 = gtk_tree_view_column_new_with_attributes("Biển số xe", renderer1, "text", 0, NULL);
gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_tang1), column1);

GtkWidget *scroll_tang1 = gtk_scrolled_window_new(NULL, NULL);
gtk_container_add(GTK_CONTAINER(scroll_tang1), treeview_tang1);
gtk_widget_set_vexpand(scroll_tang1, TRUE);
gtk_widget_set_hexpand(scroll_tang1, TRUE);

gtk_notebook_append_page(GTK_NOTEBOOK(nested_notebook), scroll_tang1, tab_tang1);

// === Tầng 2 ===
GtkWidget *tab_tang2 = gtk_label_new("Tầng 2");

// Model và TreeView cho tầng 2
GtkListStore *store_tang2 = gtk_list_store_new(1, G_TYPE_STRING);
GtkTreeIter iter2;
gtk_list_store_append(store_tang2, &iter2);
gtk_list_store_set(store_tang2, &iter2, 0, "60B2-456.78", -1);

GtkWidget *treeview_tang2 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store_tang2));
GtkCellRenderer *renderer2 = gtk_cell_renderer_text_new();
GtkTreeViewColumn *column2 = gtk_tree_view_column_new_with_attributes("Biển số xe", renderer2, "text", 0, NULL);
gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_tang2), column2);

GtkWidget *scroll_tang2 = gtk_scrolled_window_new(NULL, NULL);
gtk_container_add(GTK_CONTAINER(scroll_tang2), treeview_tang2);
gtk_widget_set_vexpand(scroll_tang2, TRUE);
gtk_widget_set_hexpand(scroll_tang2, TRUE);

gtk_notebook_append_page(GTK_NOTEBOOK(nested_notebook), scroll_tang2, tab_tang2);

// Cuối cùng: Thêm notebook con vào tab chính "Bãi xe"
gtk_notebook_append_page(GTK_NOTEBOOK(notebook), nested_notebook, tab_label3);

// Thêm notebook vào container chính
gtk_box_pack_start(GTK_BOX(containerBox), notebook, TRUE, TRUE, 5);

// Thêm vào cửa sổ & hiển thị
gtk_container_add(GTK_CONTAINER(window), containerBox);
gtk_widget_show_all(window);

}

// Hàm xử lý nhập biển số
static void onNhapBienSoXe(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;

    GtkWindow *parent_window = GTK_WINDOW(data); // truyền window vào làm cha

    dialog = gtk_dialog_new_with_buttons("Thêm",
                                         parent_window,
                                         GTK_DIALOG_MODAL,
                                         "_OK", GTK_RESPONSE_OK,
                                         "_Hủy", GTK_RESPONSE_CANCEL,
                                         NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "VD: 59A1-123.45");
    gtk_container_add(GTK_CONTAINER(content_area), entry);

    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_OK) {
        const gchar *input = gtk_entry_get_text(GTK_ENTRY(entry));
        g_print("Biển số nhập: %s\n", input);
    }

    gtk_widget_destroy(dialog);
}

// Hàm xử lý sự kiện khi nhấn nút
static void onClicked(GtkWidget *widget, gpointer data) {
    g_print("Đã mở\n");
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("hello.world", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
